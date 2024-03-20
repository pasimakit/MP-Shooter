// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UCommonActivatableWidget;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API ASPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	ASPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UFUNCTION()
	void OnGameReady();
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	void ShowMenu(const TSoftClassPtr<UCommonActivatableWidget>& MenuClass);
};
