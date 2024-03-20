#include "ScoreItemWidget.h"

#include "CommonBorder.h"
#include "Components/TextBlock.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Components/Image.h"

void UScoreItemWidget::SetupItemWidget(const ABlasterPlayerState* Player)
{
	if (Player)
	{
		PlayerId = Player->GetPlayerId();
		PlayerNameText->SetText(FText::FromString(Player->GetPlayerName()));
		KillText->SetText(FText::FromString(FString::SanitizeFloat(Player->GetScore(), 0)));
		DeathText->SetText(FText::FromString(FString::FromInt(Player->Defeats)));

		float KDRatio = 0.f;
		if (Player->Defeats == 0)
		{
			KDRatio = Player->GetScore() / 1;
		}
		else
		{
			KDRatio = Player->GetScore() / Player->Defeats;
		}

		KDRatioText->SetText(FText::FromString(FString::SanitizeFloat(KDRatio, 2)));
	}
}

void UScoreItemWidget::SetTextColor()
{
	FSlateColor LocalColor = FSlateColor(LocalPlayerColor);
	PlayerNameText->SetColorAndOpacity(LocalColor);
	KillText->SetColorAndOpacity(LocalColor);
	DeathText->SetColorAndOpacity(LocalColor);
	KDRatioText->SetColorAndOpacity(LocalColor);
}

void UScoreItemWidget::SetBackgroundColor(const FLinearColor Color)
{
	Border->GetDynamicMaterial()->SetVectorParameterValue("FillColor",  Color);
}
