// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/UI/Menu/SButtonBaseWidget.h"
#include "SButtonGameWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MULTIPLAYERMENU_API USButtonGameWidget : public USButtonBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	void SetGameData(const FPrimaryAssetId& GameDataAssetId);
	void UpdateButtonText(const FText& GameMapText, const FText& GameNameText, const FText& GameDescText);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* Desc;
	
};
