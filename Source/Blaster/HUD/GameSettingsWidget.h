// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameSettingsWidget.generated.h"

class UCommonTextBlock;
class USlider;
class USButtonBaseWidget;
/**
 * 
 */
UCLASS()
class BLASTER_API UGameSettingsWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
protected:
	UFUNCTION()
	void OnExitButtonClicked();
	
	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void OnResolutionPlusClicked();

	UFUNCTION()
	void OnResolutionMinusClicked();

	UFUNCTION()
	void OnGraphicsPlusClicked();

	UFUNCTION()
	void OnGraphicsMinusClicked();

	UFUNCTION()
	void OnDestroySession(bool bWasSuccesful);
	
	UFUNCTION()
	void OnPlayerLeftGame();

	UFUNCTION()
	void OnSensitivityValueChanged(float Value);

	UFUNCTION()
	void InitializeTextDisplay();

	UFUNCTION()
	void SaveSettings();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* ExitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* CloseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* ResolutionMinusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* ResolutionPlusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* GraphicsMinusButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* GraphicsPlusButton;

	UPROPERTY(meta = (BindWidget))
	USlider* SensitivitySlider;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* SensitivityText;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* GraphicsText;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* ResolutionText;

private:
	float fSliderValue;
	int32 iGraphicsIndex;
	int32 iResolutionIndex;
	FIntPoint SelectedRes;

	FIntPoint Res0 = FIntPoint(1280, 720);
	FIntPoint Res1 = FIntPoint(1600, 900);
	FIntPoint Res2 = FIntPoint(1920, 1080);
	FIntPoint Res3 = FIntPoint(2560, 1440);
	FIntPoint Res4 = FIntPoint(3840, 2160);
};