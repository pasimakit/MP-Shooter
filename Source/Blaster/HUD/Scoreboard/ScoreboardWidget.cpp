#include "ScoreboardWidget.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/ListView.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "ScoreItemWidget.h"

void UScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ShowScoreboard(false);
}

void UScoreboardWidget::ShowScoreboard(bool bShow)
{
	if (ScoreBorder)
	{
		ScoreBorder->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UScoreboardWidget::RefreshScoreboard(TArray<ABlasterPlayerState*> PlayerArray)
{
	for (ABlasterPlayerState* Player : PlayerArray)
	{
		RefreshPlayer(Player);
	}
}

void UScoreboardWidget::RefreshPlayer(ABlasterPlayerState* BPlayer)
{
	// Check if TMap contains the player, if not add it
	if (ScoreboardItems.Contains(BPlayer->GetPlayerId()))
	{
		ScoreboardItems[BPlayer->GetPlayerId()]->SetupItemWidget(BPlayer);

		if (BPlayer->GetTeam() != ETeam::ET_NoTeam)
		{
			SetTeam(BPlayer);
		}
	}
	else
	{
		APawn* Pawn = BPlayer->GetPawn();
		if (Pawn && Pawn->IsLocallyControlled())
		{
			AddPlayer(BPlayer, true);
		}
		else
		{
			AddPlayer(BPlayer);
		}
	}
}

void UScoreboardWidget::SetTeam(ABlasterPlayerState* Player)
{
	UScoreItemWidget* Item = ScoreboardItems[Player->GetPlayerId()];
	switch (Player->GetTeam())
	{
	case ETeam::ET_RedTeam:
		SecondItemList->AddChildToVerticalBox(Item);
		Item->SetBackgroundColor(BackgroundRed);
		break;
	default:
		ScoreItemList->AddChildToVerticalBox(Item);
		Item->SetBackgroundColor(BackgroundBlue);
		break;
	}
}

void UScoreboardWidget::AddPlayer(ABlasterPlayerState* Player, bool bIsLocal)
{
	if (ScoreItemClass)
	{
		UScoreItemWidget* ScoreItemWidget = CreateWidget<UScoreItemWidget>(ScoreBorder, ScoreItemClass);
		if (ScoreItemWidget)
		{
			ScoreItemWidget->SetupItemWidget(Player);
			ScoreItemList->AddChildToVerticalBox(ScoreItemWidget);
			ScoreboardItems.Add(Player->GetPlayerId(), ScoreItemWidget);

			if (bIsLocal)
			{
				ScoreItemWidget->SetTextColor();
			}
			if (Player->GetTeam() != ETeam::ET_NoTeam)
			{
				SetTeam(Player);
			}
			else
			{
				ScoreItemWidget->SetBackgroundColor(bDarkBackground ? BackgroundFFA1 : BackgroundFFA2);
				bDarkBackground = !bDarkBackground;
			}
		}
	}
}

void UScoreboardWidget::RemovePlayer(ABlasterPlayerState* Player)
{
	if (ScoreboardItems.Contains(Player->GetPlayerId()))
	{
		ScoreboardItems.Remove(Player->GetPlayerId());
	}
}


