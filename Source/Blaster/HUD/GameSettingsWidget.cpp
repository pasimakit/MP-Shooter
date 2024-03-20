// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettingsWidget.h"
#include "CommonTextBlock.h"
#include "PrimaryGameLayout.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Framework/BlasterGameInstance.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Components/Slider.h"
#include "Framework/UI/Menu/SButtonBaseWidget.h"
#include "GameFramework/GameUserSettings.h"

void UGameSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitializeTextDisplay();

	if(ExitButton)
	{
		ExitButton->OnClicked().AddUObject(this, &UGameSettingsWidget::OnExitButtonClicked);
		ExitButton->SetIsEnabled(true);
	}

	if(CloseButton)
	{
		CloseButton->OnClicked().AddUObject(this, &UGameSettingsWidget::OnCloseButtonClicked);
	}

	if(SensitivitySlider)
	{
		SensitivitySlider->OnValueChanged.AddDynamic(this, &UGameSettingsWidget::OnSensitivityValueChanged);
	}

	if(ResolutionPlusButton)
	{
		ResolutionPlusButton->OnClicked().AddUObject(this, &UGameSettingsWidget::OnResolutionPlusClicked);
	}

	if(ResolutionMinusButton)
	{
		ResolutionMinusButton->OnClicked().AddUObject(this, &UGameSettingsWidget::OnResolutionMinusClicked);
	}

	if(GraphicsPlusButton)
	{
		GraphicsPlusButton->OnClicked().AddUObject(this, &UGameSettingsWidget::OnGraphicsPlusClicked);
	}

	if(GraphicsMinusButton)
	{
		GraphicsMinusButton->OnClicked().AddUObject(this, &UGameSettingsWidget::OnGraphicsMinusClicked);
	}
}

UWidget* UGameSettingsWidget::NativeGetDesiredFocusTarget() const
{
	if(CloseButton)
	{
		return CloseButton;
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UGameSettingsWidget::InitializeTextDisplay()
{
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();

	if(UWorld* World = GetWorld())
	{
		if(const UBlasterGameInstance* BGameInstance = Cast<UBlasterGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(SensitivityText)
			{
				fSliderValue = BGameInstance->Sensitivity;
				const FString fValue = FString::Printf(TEXT("%.2f"), BGameInstance->Sensitivity);
				SensitivityText->SetText(FText::FromString(fValue));
			}

			if(GraphicsText)
			{
				FString fGraphics;
				switch(GameUserSettings->GetOverallScalabilityLevel())
				{
				case 0:
					fGraphics = "Low";
					break;
				case 1:
					fGraphics = "Medium";
					break;
				case 2:
					fGraphics = "High";
					break;
				case 3:
					fGraphics = "Epic";
					break;
				case 4:
					fGraphics = "Cinematic";
					break;
				default:
					fGraphics = "Custom";
				}
				GraphicsText->SetText(FText::FromString(fGraphics));
			}

			if(ResolutionText)
			{
				FIntPoint iResolution = GameUserSettings->GetScreenResolution();
				const FString fReso = FString::Printf(TEXT("%i x %i"), iResolution.X, iResolution.Y);
				ResolutionText->SetText(FText::FromString(fReso));
			}
		}
	}
}

void UGameSettingsWidget::SaveSettings()
{
	// Set the sensitivity in game instance
	if(UWorld* World = GetWorld())
	{
		if(UBlasterGameInstance* BlasterGameInstance = World->GetGameInstance<UBlasterGameInstance>())
		{
			BlasterGameInstance->Sensitivity = fSliderValue;
			BlasterGameInstance->SaveGame();
		}
		if (ABlasterPlayerController* PC = Cast<ABlasterPlayerController>(World->GetFirstPlayerController()))
		{
			PC->SetCommonUIInputMode(false, false, false);
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(PC->GetPawn());
			if (BlasterCharacter)
			{
				BlasterCharacter->UpdateSensitivity();
			}
		}
	}
	// save the graphics setting
	UGameUserSettings* GameSettings =  GEngine->GetGameUserSettings();
	GameSettings->SetOverallScalabilityLevel(iGraphicsIndex);
	GameSettings->SetScreenResolution(SelectedRes);
	GameSettings->ApplySettings(true);
}

void UGameSettingsWidget::OnCloseButtonClicked()
{
	SaveSettings();

	ABlasterPlayerController* FBlasterPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (FBlasterPlayerController)
	{
		FBlasterPlayerController->bIsSettingsOpen = false;
	}
	
	DeactivateWidget();
}

void UGameSettingsWidget::OnResolutionPlusClicked()
{
	iResolutionIndex++;
	if(iResolutionIndex > 4)
	{
		iResolutionIndex = 0;
	}
	if(ResolutionText)
	{
		FString fResolution;
		switch (iResolutionIndex)
		{
			case 0:
				fResolution = FString::Printf(TEXT("%i x %i"), Res0.X, Res0.Y);
				SelectedRes = Res0;
				break;
			case 1:
				fResolution = FString::Printf(TEXT("%i x %i"), Res1.X, Res1.Y);
				SelectedRes = Res1;
				break;
			case 2:
				fResolution = FString::Printf(TEXT("%i x %i"), Res2.X, Res2.Y);
				SelectedRes = Res2;
					break;
			case 3:
				fResolution = FString::Printf(TEXT("%i x %i"), Res3.X, Res3.Y);
				SelectedRes = Res3;
				break;
			case 4:
				fResolution = FString::Printf(TEXT("%i x %i"), Res4.X, Res4.Y);
				SelectedRes = Res4;
				break;
		}
		ResolutionText->SetText(FText::FromString(fResolution));
	}
}

void UGameSettingsWidget::OnResolutionMinusClicked()
{
	iResolutionIndex--;

	if(iResolutionIndex < 0)
	{
		iResolutionIndex = 4;
	}
	if(ResolutionText)
	{
		FString fResolution;
		switch (iResolutionIndex)
		{
		case 0:
			fResolution = FString::Printf(TEXT("%i x %i"), Res0.X, Res0.Y);
			SelectedRes = Res0;
			break;
		case 1:
			fResolution = FString::Printf(TEXT("%i x %i"), Res1.X, Res1.Y);
			SelectedRes = Res1;
			break;
		case 2:
			fResolution = FString::Printf(TEXT("%i x %i"), Res2.X, Res2.Y);
			SelectedRes = Res2;
			break;
		case 3:
			fResolution = FString::Printf(TEXT("%i x %i"), Res3.X, Res3.Y);
			SelectedRes = Res3;
			break;
		case 4:
			fResolution = FString::Printf(TEXT("%i x %i"), Res4.X, Res4.Y);
			SelectedRes = Res4;
			break;
		}
		ResolutionText->SetText(FText::FromString(fResolution));
	}
}

void UGameSettingsWidget::OnGraphicsPlusClicked()
{
	iGraphicsIndex++;
	if(iGraphicsIndex > 4)
	{
		iGraphicsIndex = 0;
	}
	if(GraphicsText)
	{
		FString fGraphics;
		switch(iGraphicsIndex)
		{
		case 0:
			fGraphics = "Low";
			break;
		case 1:
			fGraphics = "Medium";
			break;
		case 2:
			fGraphics = "High";
			break;
		case 3:
			fGraphics = "Epic";
			break;
		case 4:
			fGraphics = "Cinematic";
			break;
		}
		GraphicsText->SetText(FText::FromString(fGraphics));
	}
}

void UGameSettingsWidget::OnGraphicsMinusClicked()
{
	iGraphicsIndex--;
	if(iGraphicsIndex < 0)
	{
		iGraphicsIndex = 4;
	}
	
	if(GraphicsText)
	{
		FString fGraphics;
		switch(iGraphicsIndex)
		{
		case 0:
			fGraphics = "Low";
			break;
		case 1:
			fGraphics = "Medium";
			break;
		case 2:
			fGraphics = "High";
			break;
		case 3:
			fGraphics = "Epic";
			break;
		case 4:
			fGraphics = "Cinematic";
			break;
		}
		GraphicsText->SetText(FText::FromString(fGraphics));
	}
}

void UGameSettingsWidget::OnExitButtonClicked()
{
	ExitButton->SetIsEnabled(false);

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
				BlasterCharacter->OnLeftGame.AddDynamic(this, &UGameSettingsWidget::OnPlayerLeftGame);
			}
			else
			{
				ExitButton->SetIsEnabled(true);
			}
		}
	}
}

void UGameSettingsWidget::OnDestroySession(bool bWasSuccesful)
{
	if (!bWasSuccesful)
	{
		ExitButton->SetIsEnabled(true);
		return;
	}
	
	if (const UWorld* World = GetWorld())
	{
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
        	ExitButton->SetIsEnabled(false);
            PlayerController->ClientReturnToMainMenuWithTextReason(FText());
        }
	}
}

void UGameSettingsWidget::OnPlayerLeftGame()
{
	OnDestroySession(true);
}

void UGameSettingsWidget::OnSensitivityValueChanged(float Value)
{
		if (SensitivityText)
		{
			fSliderValue = Value;
			FString fValue = FString::Printf(TEXT("%.2f"), Value);
			SensitivityText->SetText(FText::FromString(fValue));
		}
}
