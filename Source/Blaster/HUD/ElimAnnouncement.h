#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ElimAnnouncement.generated.h"

UCLASS()
class BLASTER_API UElimAnnouncement : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	void SetElimAnnouncementText(FString AttackerName, FString VictimName);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnnouncementText;
};
