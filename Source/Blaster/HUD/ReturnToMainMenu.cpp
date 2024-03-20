#include "ReturnToMainMenu.h"
#include "PrimaryGameLayout.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Framework/BlasterGameInstance.h"

void UReturnToMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MenuSetup();
}

void UReturnToMainMenu::MenuSetup()
{
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	if (ReturnButton && !ReturnButton->OnClicked.IsBound())
	{
		ReturnButton->OnClicked.AddDynamic(this, &UReturnToMainMenu::ReturnButtonClicked);
	}
	if (SensitivitySlider && !SensitivitySlider->OnValueChanged.IsBound())
	{
		SensitivitySlider->OnValueChanged.AddDynamic(this, &UReturnToMainMenu::SensitivitySliderValueChanged);
	}
	BlasterGameInstance = BlasterGameInstance == nullptr ? Cast<UBlasterGameInstance>(GetGameInstance()) : BlasterGameInstance;
	if (BlasterGameInstance)
	{
		MultiplayerSessionsSubsystem = BlasterGameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UReturnToMainMenu::OnDestroySession);
		}

		if (SensitivityText)
		{
			SensitivityText->SetText(FText::FromString(FString::SanitizeFloat(BlasterGameInstance->Sensitivity, 1)));
		}
	}
}

void UReturnToMainMenu::OnDestroySession(bool bWasSuccesful)
{
	if (!bWasSuccesful)
	{
		ReturnButton->SetIsEnabled(true);
		return;
	}
	UWorld* World = GetWorld();

	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UReturnToMainMenu::MenuTeardown()
{
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(PlayerController->GetPawn());
			if (BlasterCharacter)
			{
				BlasterCharacter->UpdateSensitivity();
			}
		}

		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(World))
		{
			RootLayout->FindAndRemoveWidgetFromLayer(this);
		}
	}

	BlasterGameInstance = BlasterGameInstance == nullptr ? Cast<UBlasterGameInstance>(GetGameInstance()) : BlasterGameInstance;

	if (BlasterGameInstance)
	{
		BlasterGameInstance->SaveGame();
	}
}

void UReturnToMainMenu::SensitivitySliderValueChanged(float Value)
{
	if (BlasterGameInstance)
	{
		BlasterGameInstance->Sensitivity = Value;
		if (SensitivityText)
		{
			FString fValue = FString::Printf(TEXT("%.2f"), Value);
			SensitivityText->SetText(FText::FromString(fValue));
		}
	}
}

void UReturnToMainMenu::ReturnButtonClicked()
{
	ReturnButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	
	if (World)
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(World))
		{
			RootLayout->FindAndRemoveWidgetFromLayer(this);
		}
		
		APlayerController* FirstPlayerController = World->GetFirstPlayerController();
		if (FirstPlayerController)
		{
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FirstPlayerController->GetPawn());
			if (BlasterCharacter)
			{
				BlasterCharacter->ServerLeaveGame();
				BlasterCharacter->OnLeftGame.AddDynamic(this, &UReturnToMainMenu::OnPlayerLeftGame);
			}
			else
			{
				ReturnButton->SetIsEnabled(true);
			}
		}
	}
}

void UReturnToMainMenu::OnPlayerLeftGame()
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}
