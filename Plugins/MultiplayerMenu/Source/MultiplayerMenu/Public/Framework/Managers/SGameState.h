// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

class UCommonActivatableWidget;
class USGameData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateReadyDelegate);

UCLASS()
class MULTIPLAYERMENU_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ASGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetGameData(USGameData* NewGameData);
	const TSoftClassPtr<UCommonActivatableWidget> GetMenuClass() const;
	const TSoftClassPtr<UCommonActivatableWidget> GetSettingsClass() const;
	bool ShouldShowMenu() const;
	FOnGameStateReadyDelegate OnGameStateReady;

protected:
	UFUNCTION()
	void OnRep_GameData();
	UPROPERTY(ReplicatedUsing = OnRep_GameData)
	USGameData* GameData;
};
