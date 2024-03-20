// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "SGameData.generated.h"

class UCommonUserWidget;
class UCommonActivatableWidget;
/**
 * 
 */
UCLASS()
class MULTIPLAYERMENU_API USGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Asset Settings")
	FPrimaryAssetType DataType;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(DataType, GetFName());
	}

	/** Specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	/** Extra arguments passed as URL options to the game */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData)
	TMap<FString, FString> ExtraArgs;

	/** Maximum player able to join a game */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData)
	int32 MaxPlayerCount;

	/** The name of the map*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData)
	FText GameMap;

	/** The name of the game type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData)
	FText GameName;

	/** Description of the game type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData)
	FText GameDesc;

	/** Image for the game button */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData)
	TSoftObjectPtr<UTexture2D> GameImage;

	/** If true, this will show the game menu on loading */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData)
	bool bShowMenu = true;

	/** If true, this game will show in the game list when hosting a game */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GameData)
	bool bIncludeGameList = true;

	/** UI class that is the game type display widget */
	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> MainMenuClass;

	/** UI class that is the game type display widget */
	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonUserWidget> GameDisplayWidgetClass;

	/** UI Class for settings */
	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> SettingsWidgetClass;
};
