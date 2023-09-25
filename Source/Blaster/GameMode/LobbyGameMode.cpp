// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(Subsystem);

		if (NumberOfPlayers == Subsystem->DesiredNumPublicConnections)
		{
			UWorld* World = GetWorld();

			if (World)
			{
				bUseSeamlessTravel = true;

				FString MatchType = Subsystem->DesiredMatchType;

				if (MatchType == "FFA")
				{
					World->ServerTravel(FString("/Game/Maps/FFA_Map?listen"));
				}
				else if (MatchType == "TDM")
				{
					World->ServerTravel(FString("/Game/Maps/TDM_Map?listen"));
				}
				else if (MatchType == "CTF")
				{
					World->ServerTravel(FString("/Game/Maps/CTF_Map?listen"));
				}
			}
		}
	}
}
