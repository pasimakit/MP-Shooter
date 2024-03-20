// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Managers/SGameMode.h"

#include "Engine/AssetManager.h"
#include "Framework/Data/SGameData.h"
#include "Framework/Data/StaticGameData.h"
#include "Framework/Managers/SGameState.h"
#include "Framework/Player/SPlayerController.h"
#include "Kismet/GameplayStatics.h"

ASGameMode::ASGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bGameReady = false;
}

void ASGameMode::InitGameState()
{
	Super::InitGameState();

	SessionPlayers.Empty();

	if(ASGameState* SGameState = Cast<ASGameState>(GameState))
	{
		SGameState->OnGameStateReady.AddDynamic(this, &ASGameMode::OnGameReady);
	}

	// Wait for the next frame to give time to initialize startup settings, game state etc
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleGameDataAssignment);
}

void ASGameMode::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	if(ASPlayerController* Player = Cast<ASPlayerController>(NewPlayer))
	{
		SessionPlayers.AddUnique(Player);
	}
}

void ASGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if(IsGameDataLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

bool ASGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	if(bGameReady)
	{
		return Super::PlayerCanRestart_Implementation(Player);
	}
	
	return false;
}

void ASGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	if(bGameReady)
	{
		if(APlayerController* NewPC = Cast<APlayerController>(NewPlayer))
		{
			if(PlayerCanRestart(NewPC))
			{
				RequestPlayerRestartNextFrame(NewPlayer, false);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("FailedToRestartPlayer(%s) and PlayerCanRestart returned false, so we're not going try again."), *GetPathNameSafe(NewPlayer));
			}
		}
	}
	else
	{
		RequestPlayerRestartNextFrame(NewPlayer, false);
	}
}

void ASGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if(bForceReset && (Controller != nullptr))
	{
		Controller->Reset();
	}

	if(APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
}

void ASGameMode::HandleGameDataAssignment()
{
	// Check options string from multiplayer hosting
	if(!CurrentGameDataAssetId.IsValid() && UGameplayStatics::HasOption(OptionsString, S_MP_SETTINGS_GAMEMODE))
	{
		const FString GameDataFromOptions = UGameplayStatics::ParseOption(OptionsString, S_MP_SETTINGS_GAMEMODE);
		CurrentGameDataAssetId = FPrimaryAssetId(FPrimaryAssetType(USGameData::StaticClass()->GetFName()), FName(*GameDataFromOptions));
	}
	
	// Fallback to default grid

	if(!CurrentGameDataAssetId.IsValid())
	{
		CurrentGameDataAssetId = FPrimaryAssetId(FPrimaryAssetType(S_DATA_ASSET_TYPE_GAMEDATA), S_DATA_ASSET_TYPE_GAMEDATA_DEFAULT);
	}
	
	if(CurrentGameDataAssetId.IsValid())
	{
		OnGameDataAssignment();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[ASGameMode::HandleDataAssignment] Failed to identify game data!"));
	}
}

void ASGameMode::OnGameDataAssignment()
{
	if(UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
	{
		if(CurrentGameDataAssetId.IsValid())
		{
			TArray<FName> Bundles;
			Bundles.Add(S_DATA_ASSET_BUNDLE_GAME);
			const FStreamableDelegate GroupDataLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ASGameMode::OnGameDataLoaded);
			AssetManager->LoadPrimaryAsset(CurrentGameDataAssetId, Bundles, GroupDataLoadedDelegate);
		}
	}
}

void ASGameMode::OnGameDataLoaded()
{
	if(GameState && CurrentGameDataAssetId.IsValid())
	{
		if(ASGameState* SGameState = Cast<ASGameState>(GameState))
		{
			if(USGameData* GameData = GetGameData())
			{
				SGameState->SetGameData(GameData);
			}
		}
	}
}

USGameData* ASGameMode::GetGameData() const
{
	if(CurrentGameDataAssetId.IsValid())
	{
		if(const UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			return Cast<USGameData>(AssetManager->GetPrimaryAssetObject(CurrentGameDataAssetId));
		}
	}
	return nullptr;
}

void ASGameMode::OnGameReady()
{
	bGameReady = true;
	InitializePlayers();
}

void ASGameMode::InitializePlayers()
{
	for(int i = 0; i < SessionPlayers.Num(); ++i)
	{
		if((SessionPlayers[i] != nullptr) && (SessionPlayers[i]->GetPawn() == nullptr))
		{
			if(const USGameData* GameData = GetGameData())
			{
				if(PlayerCanRestart(SessionPlayers[i]))
				{
					RestartPlayer(SessionPlayers[i]);
				}
			}
		}
	}
}
