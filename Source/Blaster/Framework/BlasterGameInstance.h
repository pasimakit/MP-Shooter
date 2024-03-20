// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "BlasterGameInstance.generated.h"

class UBSaveGame;

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

public:
	UBlasterGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(BlueprintReadOnly)
	FString SaveGameSlotName;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	UBSaveGame* SaveGameObject;

	UPROPERTY()
	float Sensitivity = 1.f;

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void SaveGame();
};
