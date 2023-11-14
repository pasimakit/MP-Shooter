// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& HitTarget) override;

	UFUNCTION(Server, Reliable)
	void ServerSpawnBulletHole(const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnBulletHole(const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float DecalLifespan = 15.f;

protected:
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* ImpactDecal;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

private:
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;
};
