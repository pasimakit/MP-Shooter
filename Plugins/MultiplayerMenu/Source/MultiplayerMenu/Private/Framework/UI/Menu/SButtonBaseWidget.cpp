// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/UI/Menu/SButtonBaseWidget.h"
#include "CommonTextBlock.h"
#include "CommonBorder.h"
#include "Framework/UI/Menu/StaticMaterial.h"

void USButtonBaseWidget::SetButtonText(const FText& InText)
{
	bOverride_ButtonText = InText.IsEmpty();
	ButtonText = InText;
	UpdateButtonText(ButtonText);
}

void USButtonBaseWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitButton();
}

void USButtonBaseWidget::InitButton()
{
	OnHovered().AddUObject(this, &USButtonBaseWidget::OnButtonHovered);
	OnUnhovered().AddUObject(this, &USButtonBaseWidget::OnButtonUnHovered);

	ApplyMaterial();
	UpdateButtonText(ButtonText);
	SetButtonSettings();
}

void USButtonBaseWidget::OnButtonHovered()
{
	if(ButtonBorder != nullptr)
	{
		if(UMaterialInstanceDynamic* MaterialInstance = ButtonBorder->GetDynamicMaterial())
		{
				MaterialInstance->SetScalarParameterValue(S_MAT_PARAM_NAME_HOVER, 1.f);
		}
	}
}

void USButtonBaseWidget::OnButtonUnHovered()
{
	if(ButtonBorder != nullptr)
	{
		if(UMaterialInstanceDynamic* MaterialInstance = ButtonBorder->GetDynamicMaterial())
		{
			MaterialInstance->SetScalarParameterValue(S_MAT_PARAM_NAME_HOVER, 0.f);
		}
	}
}

void USButtonBaseWidget::ApplyMaterial() const
{
	if(ButtonBorder != nullptr && Material != nullptr)
	{
		ButtonBorder->SetBrushFromMaterial(Material);
	}
}

void USButtonBaseWidget::UpdateButtonText(const FText& InText)
{
	if(ButtonTextBlock)
	{
		ButtonTextBlock->SetText(InText);
	}
}

void USButtonBaseWidget::SetButtonSettings() const
{
	if(ButtonBorder != nullptr)
	{
		if(UMaterialInstanceDynamic* MaterialInstance = ButtonBorder->GetDynamicMaterial())
		{
			if(bOverride_Texture_Alpha > 0)
			{
				MaterialInstance->SetScalarParameterValue(S_MAT_PARAM_NAME_TEX_ALPHA, TextureAlpha);
				MaterialInstance->SetScalarParameterValue(S_MAT_PARAM_NAME_TEX_ALPHA_HOVER, TextureHoverAlpha);
			}

			if(bOverride_Texture_Scale > 0)
			{
				MaterialInstance->SetScalarParameterValue(S_MAT_PARAM_NAME_TEX_SCALE, TextureScale);
				MaterialInstance->SetScalarParameterValue(S_MAT_PARAM_NAME_TEX_SCALE_HOVER, TextureHoverScale);
			}

			if(bOverride_Texture_Shift > 0)
			{
				MaterialInstance->SetScalarParameterValue(S_MAT_PARAM_NAME_TEX_SHIFT_X, TextureShiftX);
				MaterialInstance->SetScalarParameterValue(S_MAT_PARAM_NAME_TEX_SHIFT_Y, TextureShiftY);
			}

			if(ButtonTexture != nullptr)
			{
				MaterialInstance->SetTextureParameterValue(S_MAT_PARAM_NAME_TEXTURE, ButtonTexture);
			}
		}
	}
}
