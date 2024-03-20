// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/Menu/Multiplayer/SGameDisplayListWidget.h"

#include "Components/VerticalBox.h"
#include "Engine/AssetManager.h"
#include "Framework/Data/SGameData.h"
#include "Framework/Data/StaticGameData.h"
#include "Framework/UI/Menu/Multiplayer/SGameDisplayWidget.h"

void USGameDisplayListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadGameList();
}

TArray<UWidget*> USGameDisplayListWidget::GetGameListWidgetList() const
{
	if(GameList)
	{
		return GameList->GetAllChildren();
	}

	return TArray<UWidget*>();
}

void USGameDisplayListWidget::LoadGameList()
{
	if(GameList)
	{
		GameList->ClearChildren();

		if(UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			TArray<FPrimaryAssetId> GameDataAssets;
			const FPrimaryAssetType GroupAssetType(S_DATA_ASSET_TYPE_GAMEDATA);

			AssetManager->GetPrimaryAssetIdList(GroupAssetType, GameDataAssets);

			if(GameDataAssets.Num() > 0)
			{
				const TArray<FName> Bundles;
				const FStreamableDelegate GroupDataLoadedDelegate = FStreamableDelegate::CreateUObject(this, &USGameDisplayListWidget::OnGameDataLoaded, GameDataAssets);
				AssetManager->LoadPrimaryAssets(GameDataAssets, Bundles, GroupDataLoadedDelegate);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[USGameDisplayList::LoadGameList] - Failed to get GameDataAssets Assets"))
			}
		}
	}
}

void USGameDisplayListWidget::OnGameDataLoaded(TArray<FPrimaryAssetId> GameDataAssets)
{
	if(!GetOwningPlayer() || !GetOwningPlayer()->GetWorld())
	{
		return;
	}

	if(const UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
	{
		for(int i = 0; i < GameDataAssets.Num(); ++i)
		{
			if(const USGameData* GameData = Cast<USGameData>(AssetManager->GetPrimaryAssetObject(GameDataAssets[i])))
			{
				if(GameData->bIncludeGameList)
				{
					if(GameData->GameDisplayWidgetClass.IsValid())
					{
						if(USGameDisplayWidget* GameDisplay = CreateWidget<USGameDisplayWidget>(GetOwningPlayer()->GetWorld(), GameData->GameDisplayWidgetClass.LoadSynchronous()))
						{
							GameDisplay->SetGameData(GameDataAssets[i]);
							GameList->AddChildToHorizontalBox(GameDisplay);
						}
					}
				}
			}
		}
	}

	OnGameListCreation();
}

void USGameDisplayListWidget::OnGameListCreation()
{
	OnGameListCreated.Broadcast();

	TArray<UWidget*> DisplayWidgets = GetGameListWidgetList();
	for(int i = 0; i < DisplayWidgets.Num(); ++i)
	{
		if(USGameDisplayWidget* DisplayWidget = Cast<USGameDisplayWidget>(DisplayWidgets[i]))
		{
			DisplayWidget->OnGameDisplaySelected.AddDynamic(this, &USGameDisplayListWidget::OnGameDisplaySelection);
		}
	}
}

void USGameDisplayListWidget::OnGameDisplaySelection(const FPrimaryAssetId& SelectedGameData)
{
	OnGameSelected.Broadcast(SelectedGameData);
}
