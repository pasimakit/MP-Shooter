#include "FlagZone.h"
#include "Components/SphereComponent.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/GameMode/CaptureTheFlagGameMode.h"

AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Zone Sphere"));
	SetRootComponent(ZoneSphere);
}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();
	
	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlagZone::OnSphereOverlap);
}

void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlag* OverlappingFlag = Cast<AFlag>(OtherActor);
	if (OverlappingFlag)
	{
		if (OverlappingFlag && OverlappingFlag->GetTeam() != Team)
		{
			ACaptureTheFlagGameMode* CTFGameMode = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>();
			if (CTFGameMode)
			{
				CTFGameMode->FlagCaptured(OverlappingFlag, this);
			}
			OverlappingFlag->ResetFlag();
		}
	}
}

