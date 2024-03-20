// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SMenuMultiplayerWidget.generated.h"


class USMenuJoinSessionWidget;
class USMenuHostSessionWidget;
class USButtonBaseWidget;

UCLASS(Abstract)
class MULTIPLAYERMENU_API USMenuMultiplayerWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

protected:
	UFUNCTION()
	void OnHostButtonClicked();

	UFUNCTION()
	void OnJoinButtonClicked();

	UFUNCTION()
	void OnSettingsButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* HostButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* JoinButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* SettingsButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USButtonBaseWidget* ExitButton;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftClassPtr<USMenuHostSessionWidget> HostWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftClassPtr<USMenuJoinSessionWidget> JoinWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> GameSettingsClass;
};
