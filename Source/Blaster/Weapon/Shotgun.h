// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()
public:
	virtual void FireShotgun(const TArray<FVector_NetQuantize>& HitTargets);
	void ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets);

	UFUNCTION(Server, Reliable)
	void ServerSpawnBulletHoles(const TArray<FHitResult>& Hits);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnBulletHoles(const TArray<FHitResult>& Hits);

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	uint32 NumberOfPellets = 10;
};
