// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonButtonBase.h"
#include "SButtonBaseWidget.generated.h"

class UCommonTextBlock;
class UCommonBorder;

UCLASS(Abstract)
class MULTIPLAYERMENU_API USButtonBaseWidget : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);
	
protected:
	virtual void NativePreConstruct() override;
	void InitButton();
	void OnButtonHovered();
	void OnButtonUnHovered();

	void ApplyMaterial() const;
	void UpdateButtonText(const FText& InText);
	void SetButtonSettings() const;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ButtonTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* ButtonBorder;

	UPROPERTY(EditAnywhere, Category="Button", meta=(InlineEditConditionToggle))
	uint8 bOverride_ButtonText : 1;

	UPROPERTY(EditAnywhere, Category="Button", meta=(EditCondition="bOverride_ButtonText"))
	FText ButtonText;

	UPROPERTY(EditAnywhere, Category="Button")
	UMaterialInstance* Material;

	UPROPERTY(EditAnywhere, Category="Button|Texture")
	UTexture2D* ButtonTexture;

	UPROPERTY(EditAnywhere, Category="Button", meta=(InlineEditConditionToggle))
	uint8 bOverride_Texture_Alpha : 1;

	UPROPERTY(EditAnywhere, Category="Button", meta=(EditCondition="bOverride_Texture_Alpha"))
	float TextureAlpha = 1.f;

	UPROPERTY(EditAnywhere, Category="Button", meta=(EditCondition="bOverride_Texture_Alpha"))
	float TextureHoverAlpha = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Button", meta=(InlineEditConditionToggle))
	uint8 bOverride_Texture_Scale : 1;

	UPROPERTY(EditAnywhere, Category="Button", meta=(EditCondition="bOverride_Texture_Scale"))
	float TextureScale = 0.75f;

	UPROPERTY(EditAnywhere, Category="Button", meta=(EditCondition="bOverride_Texture_Scale"))
	float TextureHoverScale = 1.f;

	UPROPERTY(EditAnywhere, Category="Button", meta=(InlineEditConditionToggle))
	uint8 bOverride_Texture_Shift : 1;;
	
	UPROPERTY(EditAnywhere, Category="Button", meta=(EditCondition="bOverride_Texture_Shift"))
	float TextureShiftX = 0.f;

	UPROPERTY(EditAnywhere, Category="Button", meta=(EditCondition="bOverride_Texture_Shift"))
	float TextureShiftY = 0.f;
	
};
