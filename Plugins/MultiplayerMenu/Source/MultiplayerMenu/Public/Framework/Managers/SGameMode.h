// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

class ASPlayerController;
class USGameData;


UCLASS()
class MULTIPLAYERMENU_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void InitGameState() override;
	virtual void GenericPlayerInitialization(AController* NewPlayer) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);
	
protected:
	void HandleGameDataAssignment();
	void OnGameDataAssignment();
	void OnGameDataLoaded();
	bool IsGameDataLoaded() const { return CurrentGameDataAssetId.IsValid(); }
	USGameData* GetGameData() const;
	
	UFUNCTION()
	void OnGameReady();

	UFUNCTION()
	void InitializePlayers();
	
	UPROPERTY()
	bool bGameReady = false;

	UPROPERTY()
	FPrimaryAssetId CurrentGameDataAssetId;

	UPROPERTY()
	TArray<ASPlayerController*> SessionPlayers;
};
