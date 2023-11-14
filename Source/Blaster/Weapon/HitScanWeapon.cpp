// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "WeaponTypes.h"
#include "Blaster/BlasterComponents/LagCompensationComponent.h"
#include "Components/DecalComponent.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (Owner == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());

		if (BlasterCharacter && InstigatorController)
		{
			bool bCauseAuthDamage = OwnerPawn->IsLocallyControlled() || !bUseServerSideRewind;
			if (HasAuthority() && bCauseAuthDamage)
			{
				const float DamageToCause = FireHit.BoneName.ToString() == FString("head") ? HeadshotDamage : Damage;

				UGameplayStatics::ApplyDamage(
					BlasterCharacter,
					DamageToCause,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}
			if(!HasAuthority() && bUseServerSideRewind && BlasterOwnerCharacter->IsLocallyControlled())
			{
				BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
				BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
				if (BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensation())
				{
					BlasterOwnerCharacter->GetLagCompensation()->ServerScoreRequest(
						BlasterCharacter,
						Start,
						HitTarget,
						BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime
					);
				}
			}
		}
		else
		{
			if (FireHit.bBlockingHit && HasAuthority())
			{
				ServerSpawnBulletHole(FireHit);
			}
		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ImpactParticles,
				FireHit.ImpactPoint,
				FireHit.ImpactNormal.Rotation()
			);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				HitSound,
				FireHit.ImpactPoint
			);
		}
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MuzzleFlash,
				SocketTransform
			);
		}
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				FireSound,
				GetActorLocation()
			);
		}
	}
}

void AHitScanWeapon::ServerSpawnBulletHole_Implementation(const FHitResult& Hit)
{
	MulticastSpawnBulletHole(Hit);
}

void AHitScanWeapon::MulticastSpawnBulletHole_Implementation(const FHitResult& Hit)
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

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FVector End = TraceStart + (HitTarget - TraceStart) * 1.25f;

		World->LineTraceSingleByChannel(
			OutHit,
			TraceStart,
			End,
			ECollisionChannel::ECC_Visibility
		);
		FVector BeamEnd = End;
		if (OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}
		else
		{
			OutHit.ImpactPoint = End;
		}

		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World,
				BeamParticles,
				TraceStart,
				FRotator::ZeroRotator,
				true
			);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}
