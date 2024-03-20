#include "Framework/UI/Menu/Multiplayer/SGameSessionEntry.h"
#include "CommonGameInstance.h"
#include "CommonSessionSubsystem.h"
#include "Engine/AssetManager.h"
#include "Framework/Data/SGameData.h"
#include "Framework/Data/StaticGameData.h"
#include "Framework/UI/Menu/Multiplayer/SGameSessionButton.h"


void USGameSessionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(SessionButton)
	{
		SessionButton->OnClicked().AddUObject(this, &USGameSessionEntry::OnSessionSelected);
	}
}

void USGameSessionEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	SessionSearchResult = Cast<UCommonSession_SearchResult>(ListItemObject);

	if(SessionSearchResult != nullptr)
	{
		FString GameDataName;
		bool bDataFound;
		SessionSearchResult->GetStringSetting(S_MP_SETTINGS_GAMEMODE, GameDataName, bDataFound);

		if(bDataFound)
		{
			const FPrimaryAssetType Type(S_DATA_ASSET_TYPE_GAMEDATA);
			GameDataId = FPrimaryAssetId(Type, FName(*GameDataName));

			if(GameDataId.IsValid())
			{
				if(UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
				{
					const TArray<FName> Bundles;
					const FStreamableDelegate DataLoadedDelegate = FStreamableDelegate::CreateUObject(this, &USGameSessionEntry::OnGameDataLoaded);
					AssetManager->LoadPrimaryAsset(GameDataId, Bundles, DataLoadedDelegate);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[USGameSessionEntry::NativeOnListItemObjectSet] - GameDataAssetId is not valid!"));
			}
		}
	}
}

void USGameSessionEntry::OnSessionSelected()
{
	if(!GetOwningPlayer() || !GetOwningPlayer()->GetWorld()) return;

	if(UCommonGameInstance* GameInstance = Cast<UCommonGameInstance>(GetOwningPlayer()->GetWorld()->GetGameInstance()))
	{
		if(UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>())
		{
			JoinSessionHandle = SessionSubsystem->OnJoinSessionCompleteEvent.AddUObject(this, &USGameSessionEntry::OnJoinSessionComplete);
			SessionSubsystem->JoinSession(GetOwningPlayer(), SessionSearchResult);
		}
	}
}

void USGameSessionEntry::OnJoinSessionComplete(const FOnlineResultInformation& Result)
{
	if(!GetOwningPlayer() || !GetOwningPlayer()->GetWorld()) return;

	if(Result.bWasSuccessful)
	{
		if(UCommonGameInstance* GameInstance = Cast<UCommonGameInstance>(GetOwningPlayer()->GetWorld()->GetGameInstance()))
		{
			if(UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>())
			{
				SessionSubsystem->OnJoinSessionCompleteEvent.Remove(JoinSessionHandle);
			}
		}
	}
}

void USGameSessionEntry::OnGameDataLoaded()
{
	if(GameDataId.IsValid())
	{
		if(const UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			if(const USGameData* GameData = Cast<USGameData>(AssetManager->GetPrimaryAssetObject(GameDataId)))
			{
				if(SessionButton)
				{
					SessionButton->SetTextDisplays(
						GameData->GameName,
						GameData->GameMap,
						FText::AsNumber(SessionSearchResult->GetPingInMs()),
						FText::AsNumber(SessionSearchResult->GetMaxPublicConnections() - SessionSearchResult->GetNumOpenPublicConnections()),
						FText::AsNumber(SessionSearchResult->GetMaxPublicConnections())
						);
				}
			}
		}
	}
}
