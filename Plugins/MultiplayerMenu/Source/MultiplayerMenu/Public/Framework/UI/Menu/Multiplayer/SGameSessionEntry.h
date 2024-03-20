// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "CommonUserTypes.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SGameSessionEntry.generated.h"

class UCommonSession_SearchResult;
class USGameSessionButton;

UCLASS()
class MULTIPLAYERMENU_API USGameSessionEntry : public UCommonTabListWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UFUNCTION()
	void OnSessionSelected();
	
	UFUNCTION()
	void OnJoinSessionComplete(const FOnlineResultInformation& Result);

	UFUNCTION()
	void OnGameDataLoaded();

	UPROPERTY()
	UCommonSession_SearchResult* SessionSearchResult;

	UPROPERTY()
	FPrimaryAssetId GameDataId;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USGameSessionButton* SessionButton;

	FDelegateHandle JoinSessionHandle;

	FString FServerName;
};
