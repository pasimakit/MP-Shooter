// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ScoreItemWidget.generated.h"

class UCommonBorder;
class UImage;
class UTextBlock;
class ABlasterPlayerState;
class LinearColor;

/**
 * 
 */
UCLASS(Abstract)
class BLASTER_API UScoreItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void SetupItemWidget(const ABlasterPlayerState* Player);

	void SetTextColor();

	void SetBackgroundColor(const FLinearColor Color);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KDRatioText;

	UPROPERTY(meta = (BindWidget))
	UCommonBorder* Border;

	UPROPERTY()
	int32 PlayerId;

protected:
	UPROPERTY(EditAnywhere)
	FLinearColor LocalPlayerColor;
};
