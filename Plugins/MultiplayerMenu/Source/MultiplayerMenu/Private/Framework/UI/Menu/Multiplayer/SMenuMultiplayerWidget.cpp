// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/Menu/Multiplayer/SMenuMultiplayerWidget.h"

#include "Framework/Data/StaticGameData.h"
#include "Framework/UI/SPrimaryGameLayout.h"
#include "Framework/UI/Menu/SButtonBaseWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Framework/UI/Menu/Multiplayer/SMenuHostSessionWidget.h"

void USMenuMultiplayerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(HostButton)
	{
		HostButton->OnClicked().AddUObject(this, &USMenuMultiplayerWidget::OnHostButtonClicked);
	}

	if(JoinButton)
	{
		JoinButton->OnClicked().AddUObject(this, &USMenuMultiplayerWidget::OnJoinButtonClicked);
	}

	if(SettingsButton)
	{
		SettingsButton->OnClicked().AddUObject(this, &USMenuMultiplayerWidget::OnSettingsButtonClicked);
	}

	if(ExitButton)
	{
		ExitButton->OnClicked().AddUObject(this, &USMenuMultiplayerWidget::OnExitButtonClicked);
	}
}

UWidget* USMenuMultiplayerWidget::NativeGetDesiredFocusTarget() const
{
	if(HostButton)
	{
		return HostButton;
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

void USMenuMultiplayerWidget::OnHostButtonClicked()
{
	if(HostWidgetClass == nullptr) return;

	if(const UWorld* WorldContext = GetWorld())
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext))
		{
			RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(UILayerTags::TAG_UI_LAYER_MENU, true, HostWidgetClass,
				[this](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen)
				{
					switch (State)
					{
					case EAsyncWidgetLayerState::AfterPush:
						return;
					case EAsyncWidgetLayerState::Canceled:
						return;
					}
				});
		}
	}
}

void USMenuMultiplayerWidget::OnSettingsButtonClicked()
{
	if(GameSettingsClass == nullptr) return;

	if(const UWorld* WorldContext = GetWorld())
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext))
		{
			RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(UILayerTags::TAG_UI_LAYER_MENU, true, GameSettingsClass,
				[this](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen)
				{
					switch (State)
					{
					case EAsyncWidgetLayerState::AfterPush:
						return;
					case EAsyncWidgetLayerState::Canceled:
						return;
					}
				});
		}
	}
}

void USMenuMultiplayerWidget::OnJoinButtonClicked()
{
	if(JoinWidgetClass == nullptr) return;

	if(const UWorld* WorldContext = GetWorld())
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext))
		{
			RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(UILayerTags::TAG_UI_LAYER_MENU, true, JoinWidgetClass,
				[this](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen)
				{
					switch (State)
					{
					case EAsyncWidgetLayerState::AfterPush:
						return;
					case EAsyncWidgetLayerState::Canceled:
						return;
					}
				});
		}
	}
}

void USMenuMultiplayerWidget::OnExitButtonClicked()
{
	if(GetOwningPlayer())
	{
		if(const UWorld* WorldContext = GetWorld())
		{
			UKismetSystemLibrary::QuitGame(WorldContext, GetOwningPlayer(), EQuitPreference::Quit, false);
		}
	}
}
