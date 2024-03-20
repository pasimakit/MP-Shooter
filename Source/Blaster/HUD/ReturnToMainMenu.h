// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ReturnToMainMenu.generated.h"

class USlider;
class UTextBlock;
class UButton;
class APlayerController;
class UMultiplayerSessionsSubsystem;
class UBlasterGameInstance;
/**
 * Make this class inherit from UCommonActivatableWidget
 * Attach it to SPrimaryGameLayout -> Menu ?
 * Copy the initalization from smenuhostwidget
 * SMenuMultiplayerWidget has example for creating this widget and attachin to primarygamelayout
 * 
 * Do the same for scoreboard widget but attach it to GameMenu ?
 */
UCLASS()
class BLASTER_API UReturnToMainMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void MenuSetup();
	UFUNCTION(BlueprintCallable)
	void MenuTeardown();
	UFUNCTION()
	void SensitivitySliderValueChanged(float Value);

	UPROPERTY(meta = (BindWidget))
	USlider* SensitivitySlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SensitivityText;
protected:
	UFUNCTION()
	void OnDestroySession(bool bWasSuccesful);
	UFUNCTION()
	void OnPlayerLeftGame();
private:
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnButton;

	UFUNCTION()
	void ReturnButtonClicked();

	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UBlasterGameInstance* BlasterGameInstance;
};
