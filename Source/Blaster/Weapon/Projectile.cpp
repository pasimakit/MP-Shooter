// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Blaster.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);
}


void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}

	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
	{
		return;
	}
	ServerSpawnBulletHole(Hit);
	Destroy();
}


void AProjectile::ServerSpawnBulletHole_Implementation(const FHitResult& Hit)
{
	MulticastSpawnBulletHole(Hit);
}

void AProjectile::MulticastSpawnBulletHole_Implementation(const FHitResult& Hit)
{
	if (GetWorld())
	{
		FVector HitLocation = Hit.ImpactPoint;
		if (Hit.bBlockingHit)
		{
			if (ImpactDecal == nullptr) return;
			FRotator Rotation = Hit.ImpactNormal.Rotation();
			float RandOffset = FMath::RandRange(-180.f, 180.f);
			Rotation = FRotator(Rotation.Pitch, Rotation.Yaw, Rotation.Roll + RandOffset);

			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactDecal, FVector(12.f, 12.f, 12.f), Hit.ImpactPoint, Rotation, DecalLifespan);
			Decal->SetFadeScreenSize(0.f);
		}
	}
}

void AProjectile::ExplodeDamage()
{
	APawn* FiringPawn = GetInstigator();

	if (FiringPawn && HasAuthority())
	{
		AController* FiringController = FiringPawn->GetController();
		if (FiringController)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this,
				Damage,
				10.f,
				GetActorLocation(),
				DamageInnerRadius,
				DamageOuterRadius,
				1.f,   // Damage falloff
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				this,
				FiringController
			);
		}
	}
}

void AProjectile::SpawnTrailSystem()
{
	if (TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailSystem,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			false
		);
	}
}

void AProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(
		DestroyTimer,
		this,
		&AProjectile::DestroyTimerFinished,
		DestroyTime
	);
}

void AProjectile::DestroyTimerFinished()
{
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (TerrainHitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TerrainHitParticles, GetActorTransform());
	}
	if (TerrainImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TerrainImpactSound, GetActorLocation());
	}
}

