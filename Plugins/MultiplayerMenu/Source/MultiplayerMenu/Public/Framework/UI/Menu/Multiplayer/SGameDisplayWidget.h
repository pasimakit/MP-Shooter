// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SGameDisplayWidget.generated.h"

class USButtonGameWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSelectDisplayDelegate, const FPrimaryAssetId&, SelectedGameData);

UCLASS(Abstract)
class MULTIPLAYERMENU_API USGameDisplayWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	void SetGameData(const FPrimaryAssetId& Data);
	FPrimaryAssetId GetGameDataAsset() const { return GameDataId; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonGameWidget* GameButtonWidget;

	FOnGameSelectDisplayDelegate OnGameDisplaySelected;

protected:
	UFUNCTION()
	void OnGameSelection();
	
	UPROPERTY()
	FPrimaryAssetId GameDataId;
};
