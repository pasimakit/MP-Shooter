// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMainMenu.generated.h"

class USlider;
class UTextBlock;
class UButton;
class APlayerController;
class UMultiplayerSessionsSubsystem;
class UBlasterGameInstance;
/**
 * 
 */
UCLASS()
class BLASTER_API UReturnToMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
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
	virtual bool Initialize() override;
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
