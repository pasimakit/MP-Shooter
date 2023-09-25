#include "PickupSpawnpoint.h"
#include "Pickup.h"

APickupSpawnpoint::APickupSpawnpoint()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APickupSpawnpoint::BeginPlay()
{
	Super::BeginPlay();

	SpawnPickup();
}

void APickupSpawnpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupSpawnpoint::SpawnPickup()
{
	int32 NumPickupClasses = PickupClasses.Num();
	if (NumPickupClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);
		SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform());

		if (HasAuthority() && SpawnedPickup)
		{
			SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupSpawnpoint::StartSpawnPickupTimer);
		}
	}
}

void APickupSpawnpoint::SpawnPickupTimerFinished()
{
	if (HasAuthority())
	{
		SpawnPickup();
	}
}

void APickupSpawnpoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);

	GetWorldTimerManager().SetTimer(
		SpawnPickupTimer,
		this,
		&APickupSpawnpoint::SpawnPickupTimerFinished,
		SpawnTime
	);
}

