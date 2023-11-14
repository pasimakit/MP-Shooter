// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blaster/BlasterComponents/LagCompensationComponent.h"
#include "Components/DecalComponent.h"

void AShotgun::FireShotgun(const TArray<FVector_NetQuantize>& HitTargets)
{
	AWeapon::Fire(FVector());

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (Owner == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");

	if (MuzzleFlashSocket)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector Start = SocketTransform.GetLocation();

		// Maps hit characters to number of times hit
		TMap<ABlasterCharacter*, uint32> HitMap;
		TMap<ABlasterCharacter*, uint32> HeadshotHitMap;
		TArray<FHitResult> MissedHits;
		for (FVector_NetQuantize HitTarget : HitTargets)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if (BlasterCharacter)
			{
				const bool bHeadShot = FireHit.BoneName.ToString() == FString("head");

				if (bHeadShot)
				{
					if (HeadshotHitMap.Contains(BlasterCharacter)) HeadshotHitMap[BlasterCharacter]++;
					else HeadshotHitMap.Emplace(BlasterCharacter, 1);
				}
				else
				{
					if (HitMap.Contains(BlasterCharacter)) HitMap[BlasterCharacter]++;
					else HitMap.Emplace(BlasterCharacter, 1);
				}

			}
			else
			{
				if (FireHit.bBlockingHit)
				{
					MissedHits.Add(FireHit);
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
					FireHit.ImpactPoint,
					.5f,
					FMath::FRandRange(-.5f, .5f)
				);
			}
		}
		TArray<ABlasterCharacter*> HitCharacters;

		// Maps character total damage hit
		TMap<ABlasterCharacter*, uint32> DamageMap;
		// Bodyshot damage per character
		for (auto HitPair : HitMap)
		{
			if (HitPair.Key && InstigatorController)
			{
				DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);

				HitCharacters.AddUnique(HitPair.Key);
			}
		}
		// Headshot damage per character
		for (auto HeadshotHitPair : HeadshotHitMap)
		{
			if (HeadshotHitPair.Key && InstigatorController)
			{
				if (DamageMap.Contains(HeadshotHitPair.Key)) DamageMap[HeadshotHitPair.Key] += HeadshotHitPair.Value * HeadshotDamage;
				else DamageMap.Emplace(HeadshotHitPair.Key, HeadshotHitPair.Value * HeadshotDamage);

				HitCharacters.AddUnique(HeadshotHitPair.Key);
			}
		}
		// Apply damage to characters
		for (auto DamagePair : DamageMap)
		{
			if (DamagePair.Key && InstigatorController)
			{
				bool bCauseAuthDamage = OwnerPawn->IsLocallyControlled() || !bUseServerSideRewind;
				if (HasAuthority() && bCauseAuthDamage)
				{
					UGameplayStatics::ApplyDamage(
						DamagePair.Key,
						DamagePair.Value,
						InstigatorController,
						this,
						UDamageType::StaticClass()
					);
				}
			}
		}
		if (!HasAuthority() && bUseServerSideRewind)
		{
			BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
			BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
			if (BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensation() && BlasterOwnerCharacter->IsLocallyControlled())
			{
				BlasterOwnerCharacter->GetLagCompensation()->ShotgunServerScoreRequest(
					HitCharacters,
					Start,
					HitTargets,
					BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime
				);
			}
		}

		ServerSpawnBulletHoles(MissedHits);
	}
}

void AShotgun::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket == nullptr) return;

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();

	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const float Time = RecoilTimeline.GetPlaybackPosition();
	const FVector RecoilOffset = FVector(0.f, HorizontalCurve->GetFloatValue(Time), VerticalCurve->GetFloatValue(Time));
	//const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	const FVector RecoilCenter = TraceStart + (ToTargetNormalized + RecoilOffset) * DistanceToSphere;

	for (uint32 i = 0; i < NumberOfPellets; i++)
	{
		const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
		const FVector EndLoc = RecoilCenter + RandVec;
		FVector ToEndLoc = EndLoc - TraceStart;
		ToEndLoc = TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size();

		HitTargets.Add(ToEndLoc);

		//DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, true);
		//DrawDebugLine(GetWorld(), TraceStart, FVector(TraceStart + RecoilCenter * TRACE_LENGTH / ToEndLoc.Size()), FColor::Cyan, true);
	}

	//DrawDebugSphere(GetWorld(), RecoilCenter, SphereRadius, 12, FColor::Red, true);
	
}

void AShotgun::ServerSpawnBulletHoles_Implementation(const TArray<FHitResult>& Hits)
{
	MulticastSpawnBulletHoles(Hits);
}

void AShotgun::MulticastSpawnBulletHoles_Implementation(const TArray<FHitResult>& Hits)
{
	if (GetWorld())
	{
		for (auto Hit : Hits)
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
}
