// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BlasterGameInstance.generated.h"

class UBSaveGame;

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
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
