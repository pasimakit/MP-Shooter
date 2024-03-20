// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Player/SPlayerController.h"
#include "PrimaryGameLayout.h"
#include "Blaster/Framework/BlasterGameInstance.h"
#include "Framework/Data/StaticGameData.h"
#include "Framework/Managers/SGameState.h"

ASPlayerController::ASPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(GetWorld())
	{
		if(ASGameState* SGameState = Cast<ASGameState>(GetWorld()->GetGameState()))
		{
			SGameState->OnGameStateReady.AddDynamic(this, &ASPlayerController::OnGameReady);        
		}
	}

	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	
	if (BlasterGameInstance)
	{
		BlasterGameInstance->LoadGame();
	}
}

void ASPlayerController::OnGameReady()
{
	if(GetWorld())
	{
		if(ASGameState* SGameState = Cast<ASGameState>(GetWorld()->GetGameState()))
		{
			if(IsLocalController())
			{
				if(SGameState->ShouldShowMenu())
				{
					ShowMenu(SGameState->GetMenuClass());
				}
			}

			SGameState->OnGameStateReady.RemoveDynamic(this, &ASPlayerController::OnGameReady);
		}
	}
}

void ASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(IsLocalController())
	{
		if(const UWorld* WorldContext = GetWorld())
		{
			if(const ASGameState* SGameState = Cast<ASGameState>(WorldContext->GetGameState()))
			{
				if(SGameState->ShouldShowMenu())
				{ 
					ShowMenu(SGameState->GetMenuClass());
				}
			}
		}
	}
}

void ASPlayerController::ShowMenu(const TSoftClassPtr<UCommonActivatableWidget>& MenuClass)
{
	if(MenuClass == nullptr) return;

	if(const UWorld* WorldContext = GetWorld())
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext))
		{
			RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(UILayerTags::TAG_UI_LAYER_MENU, true, MenuClass, [this](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen)
			{
				switch (State)
				{
				case EAsyncWidgetLayerState::AfterPush:
					IgnoreLookInput = true;
					IgnoreMoveInput = true;
					SetShowMouseCursor(true);
					return;
				case EAsyncWidgetLayerState::Canceled:
					return;
				}
			});
		}
	}
}