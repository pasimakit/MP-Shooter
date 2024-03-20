// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonUserSubsystem.h"
#include "CommonSessionSubsystem.h"
#include "CommonActivatableWidget.h"
#include "SMenuHostSessionWidget.generated.h"

class UEditableTextBox;
class UCommonUserInfo;
class USButtonBaseWidget;
class UCommonTextBlock;
class USGameDisplayListWidget;
class UCommonSession_HostSessionRequest;

UENUM()
enum ESetting1
{
	Set1,
	Set2,
};

UENUM()
enum ESetting2
{
	Set3,
	Set4,
};

UCLASS(Abstract)
class MULTIPLAYERMENU_API USMenuHostSessionWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

protected:
	void InitTextDisplays();
	void AttemptOnlineLogin();
	void HostSession();
	UCommonSession_HostSessionRequest* CreateHostingRequest();

	UFUNCTION()
	void OnUserOnlineLogin(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext);

	UFUNCTION()
	void OnSessionCreated(const FOnlineResultInformation& Result);

	UFUNCTION()
	void OnLaunchGame();

	UFUNCTION()
	void OnBackMenu();

	UFUNCTION()
	void OnGameListCreated();

	UFUNCTION()
	void OnGameSelected(const FPrimaryAssetId& SelectedGameData);

	UPROPERTY()
	FPrimaryAssetId GameDataId;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USGameDisplayListWidget* GameList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* SelectedGameTitleText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* LaunchButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* BackButton;

	/** NetMode Settings */
	UFUNCTION()
	void OnNetworkModeButtonClicked();
	
	UPROPERTY()
	ECommonSessionOnlineMode NetMode;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* NetworkModeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* ChangeNetModeButton;
};
