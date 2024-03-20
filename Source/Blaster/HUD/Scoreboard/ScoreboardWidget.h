// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ScoreboardWidget.generated.h"

class UVerticalBox;
class UBorder;
class ABlasterPlayerState;
class UScoreItemWidget;

struct ScoreItemObject : UObject
{
	FString PlayerName;
	float Score;
	int32 Defeats;
};

/**
 * 
 */
UCLASS()
class BLASTER_API UScoreboardWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ShowScoreboard(bool bShow);

	UFUNCTION()
	void RefreshScoreboard(TArray<ABlasterPlayerState*> PlayerArray);

	UFUNCTION()
	void RefreshPlayer(ABlasterPlayerState* BPlayer);

	UFUNCTION()
	void RemovePlayer(ABlasterPlayerState* Player);

	UFUNCTION()
	void SetTeam(ABlasterPlayerState* Player);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* ScoreItemList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* SecondItemList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* ScoreBorder;

	UPROPERTY()
	TMap<int,UScoreItemWidget*> ScoreboardItems;

protected:
	UFUNCTION()
	void AddPlayer(ABlasterPlayerState* Player, bool bIsLocal = false);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScoreItemWidget> ScoreItemClass;

	UPROPERTY()
	ABlasterPlayerState* LocalPlayerState;

	UPROPERTY(EditAnywhere)
	FLinearColor BackgroundBlue;

	UPROPERTY(EditAnywhere)
	FLinearColor BackgroundRed;

	UPROPERTY(EditAnywhere)
	FLinearColor BackgroundFFA1;

	UPROPERTY(EditAnywhere)
	FLinearColor BackgroundFFA2;

	UPROPERTY()
	bool bDarkBackground = true;
};
