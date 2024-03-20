// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/Menu/Multiplayer/SButtonGameWidget.h"

#include "CommonTextBlock.h"
#include "Engine/AssetManager.h"
#include "Framework/Data/SGameData.h"

void USButtonGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USButtonGameWidget::SetGameData(const FPrimaryAssetId& GameDataAssetId)
{
	if(GameDataAssetId.IsValid())
	{
		if(const UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			if(const USGameData* GameData = Cast<USGameData>(AssetManager->GetPrimaryAssetObject(GameDataAssetId)))
			{
				InitButton();
				UpdateButtonText(GameData->GameMap, GameData->GameName, GameData->GameDesc);
				if(GameData->GameImage.LoadSynchronous())
				{
					ButtonTexture = GameData->GameImage.LoadSynchronous();
					SetButtonSettings();
				}
			}
		}
	}
}

void USButtonGameWidget::UpdateButtonText(const FText& GameMapText, const FText& GameNameText, const FText& GameDescText)
{
	if(ButtonTextBlock)
	{
		ButtonText = GameMapText;
		SetButtonText(ButtonText);
	}
	if(Name)
	{
		Name->SetText(GameNameText);
	}
	if(Desc)
	{
		Desc->SetText(GameDescText);
	}
}
