// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Managers/SGameState.h"

#include "Framework/Data/SGameData.h"
#include "Net/UnrealNetwork.h"

ASGameState::ASGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, GameData);
}

void ASGameState::SetGameData(USGameData* NewGameData)
{
	if(HasAuthority())
	{
		if(NewGameData != nullptr)
		{
			GameData = NewGameData;
			OnRep_GameData();
		}
	}
}

const TSoftClassPtr<UCommonActivatableWidget> ASGameState::GetMenuClass() const
{
	if(GameData != nullptr)
	{
		return GameData->MainMenuClass;
	}
	
	return nullptr;
}

const TSoftClassPtr<UCommonActivatableWidget> ASGameState::GetSettingsClass() const
{
	if(GameData != nullptr)
	{
		return GameData->SettingsWidgetClass;
	}
	
	return nullptr;
}

bool ASGameState::ShouldShowMenu() const
{
	if(GameData != nullptr)
	{
		return GameData->bShowMenu;
	}

	return false;
}

void ASGameState::OnRep_GameData()
{
	OnGameStateReady.Broadcast();
}
