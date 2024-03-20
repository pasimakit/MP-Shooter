// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/Menu/Multiplayer/SMenuHostSessionWidget.h"

#include "CommonGameInstance.h"
#include "CommonTextBlock.h"
#include "CommonUserSubsystem.h"
#include "PrimaryGameLayout.h"
#include "Components/EditableTextBox.h"
#include "Framework/Data/SGameData.h"
#include "Framework/Data/StaticGameData.h"
#include "Framework/UI/Menu/SButtonBaseWidget.h"
#include "Framework/UI/Menu/Multiplayer/SGameDisplayListWidget.h"
#include "Framework/UI/Menu/Multiplayer/SGameDisplayWidget.h"

void USMenuHostSessionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	NetMode = ECommonSessionOnlineMode::Online;
	InitTextDisplays();

	if(ChangeNetModeButton)
	{
		ChangeNetModeButton->OnClicked().AddUObject(this, &USMenuHostSessionWidget::OnNetworkModeButtonClicked);
	}

	if(LaunchButton)
	{
		LaunchButton->OnClicked().AddUObject(this, &USMenuHostSessionWidget::OnLaunchGame);
	}

	if(BackButton)
	{
		BackButton->OnClicked().AddUObject(this, &USMenuHostSessionWidget::OnBackMenu);
	}

	if(GameList)
	{
		GameList->OnGameListCreated.AddDynamic(this, &USMenuHostSessionWidget::OnGameListCreated);
		GameList->OnGameSelected.AddDynamic(this, &USMenuHostSessionWidget::OnGameSelected);
	}
}

void USMenuHostSessionWidget::InitTextDisplays()
{
	if(NetworkModeText)
	{
		NetworkModeText->SetText(UEnum::GetDisplayValueAsText(NetMode));
	}
}

void USMenuHostSessionWidget::AttemptOnlineLogin()
{
	if(!GetOwningPlayer() || !GetOwningPlayer()->GetWorld()) return;

	if(const UCommonGameInstance* GameInstance = Cast<UCommonGameInstance>(GetOwningPlayer()->GetWorld()->GetGameInstance()))
	{
		if(UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>())
		{
			UserSubsystem->OnUserInitializeComplete.AddDynamic(this, &USMenuHostSessionWidget::OnUserOnlineLogin);
			UserSubsystem->TryToLoginForOnlinePlay(0);
		}
	}
}

void USMenuHostSessionWidget::HostSession()
{
	if(!GetOwningPlayer() || !GetOwningPlayer()->GetWorld()) return;

	if(const UCommonGameInstance* GameInstance = Cast<UCommonGameInstance>(GetOwningPlayer()->GetWorld()->GetGameInstance()))
	{
		if(UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>())
		{
			SessionSubsystem->OnCreateSessionCompleteEvent.AddUObject(this, &USMenuHostSessionWidget::OnSessionCreated);
			UCommonSession_HostSessionRequest* Request = CreateHostingRequest();
			SessionSubsystem->HostSession(GetOwningPlayer(), Request);
		}
	}
}

UCommonSession_HostSessionRequest* USMenuHostSessionWidget::CreateHostingRequest()
{
	UCommonSession_HostSessionRequest* Request = NewObject<UCommonSession_HostSessionRequest>();

	if(GameDataId.IsValid())
	{
		if(const UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			if(const USGameData* GameData = Cast<USGameData>(AssetManager->GetPrimaryAssetObject(GameDataId)))
			{
				const FString GameDataName = GameDataId.PrimaryAssetName.ToString();
				Request->ModeNameForAdvertisement = GameDataName;
				Request->OnlineMode = ECommonSessionOnlineMode::Online;
				Request->bUseLobbies = true;
				Request->MapID = GameData->MapID;
				Request->ExtraArgs = GameData->ExtraArgs;
				Request->ExtraArgs.Add(S_MP_SETTINGS_GAMEMODE, GameDataName);
				Request->MaxPlayerCount = GameData->MaxPlayerCount;
			}
		}
	}
	
	return Request;
}

void USMenuHostSessionWidget::OnUserOnlineLogin(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
                                                ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext)
{
	if(bSuccess)
	{
		HostSession();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[USMenuHostSessionWidget::OnUserOnlineLogin] Failed to login online"));
	}
}

void USMenuHostSessionWidget::OnSessionCreated(const FOnlineResultInformation& Result)
{
	if(Result.bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[USMenuHostSessionWidget::OnSessionCreated] Session created"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[USMenuHostSessionWidget::OnSessionCreated] Failed to create session"));
	}
}

void USMenuHostSessionWidget::OnLaunchGame()
{
	AttemptOnlineLogin();
}

void USMenuHostSessionWidget::OnBackMenu()
{
	if(const UWorld* WorldContext = GetWorld())
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext))
		{
			RootLayout->FindAndRemoveWidgetFromLayer(this);
		}
	}
}

void USMenuHostSessionWidget::OnGameListCreated()
{
	TArray<UWidget*> DisplayWidgets = GameList->GetGameListWidgetList();
	if(DisplayWidgets.IsValidIndex(0))
	{
		if(const USGameDisplayWidget* DisplayWidget = Cast<USGameDisplayWidget>(DisplayWidgets[0]))
		{
			const FPrimaryAssetId& DisplayGameData = DisplayWidget->GetGameDataAsset();
			OnGameSelected(DisplayGameData);
		}
	}
}

void USMenuHostSessionWidget::OnGameSelected(const FPrimaryAssetId& SelectedGameData)
{
	if(!SelectedGameData.IsValid())
	{
		return;
	}

	GameDataId = SelectedGameData;

	if(const UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
	{
		if(const USGameData* GameData = Cast<USGameData>(AssetManager->GetPrimaryAssetObject(SelectedGameData)))
		{
			if(SelectedGameTitleText && GameData)
			{
				SelectedGameTitleText->SetText(GameData->GameName);
			}
		}
	}
}

void USMenuHostSessionWidget::OnNetworkModeButtonClicked()
{
	switch(NetMode)
	{
	case ECommonSessionOnlineMode::Offline:
		NetMode = ECommonSessionOnlineMode::LAN;
		break;
	case ECommonSessionOnlineMode::LAN:
		NetMode = ECommonSessionOnlineMode::Online;
		break;
	case ECommonSessionOnlineMode::Online:
		NetMode = ECommonSessionOnlineMode::Offline;
		break;
	default: NetMode = ECommonSessionOnlineMode::LAN;
	}

	if(NetworkModeText)
	{
		NetworkModeText->SetText(UEnum::GetDisplayValueAsText(NetMode));
	}
}
