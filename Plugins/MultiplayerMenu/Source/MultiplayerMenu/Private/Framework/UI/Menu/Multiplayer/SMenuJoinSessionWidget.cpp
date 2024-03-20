// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/UI/Menu/Multiplayer/SMenuJoinSessionWidget.h"

#include "CommonBorder.h"
#include "CommonGameInstance.h"
#include "PrimaryGameLayout.h"
#include "Framework/UI/Menu/SButtonBaseWidget.h"
#include "CommonListView.h"
#include "CommonSessionSubsystem.h"

void USMenuJoinSessionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(RefreshButton)
	{
		RefreshButton->OnClicked().AddUObject(this, &USMenuJoinSessionWidget::OnRefreshList);	
	}

	if(BackButton)
	{
		BackButton->OnClicked().AddUObject(this, &USMenuJoinSessionWidget::OnBackMenu);
	}

	SetSpinnerDisplay(false);
}

void USMenuJoinSessionWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	StartSearch();
}

UWidget* USMenuJoinSessionWidget::NativeGetDesiredFocusTarget() const
{
	if(ListView)
	{
		return ListView;
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

void USMenuJoinSessionWidget::StartSearch()
{
	UE_LOG(LogTemp, Warning, TEXT("[USMenuJoinSessionWidget::StartSearch] - Start search"))

	if(bSearchInProgress || !GetOwningPlayer() || !GetOwningPlayer()->GetWorld() || !ListView)
	{
		UE_LOG(LogTemp, Warning, TEXT("[USMenuJoinSessionWidget::StartSeach] - Search failed"))
		return;
	}

	bSearchInProgress = true;
	NoSessionsDisplay->SetVisibility(ESlateVisibility::Collapsed);
	ListView->ClearListItems();
	SetSpinnerDisplay(true);

	if(UCommonGameInstance* GameInstance = Cast<UCommonGameInstance>(GetOwningPlayer()->GetWorld()->GetGameInstance()))
	{
		if(UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>())
		{
			if(UCommonSession_SearchSessionRequest* SessionSearchRequest = SessionSubsystem->CreateOnlineSearchSessionRequest())
			{
				SessionSearchRequest->OnSearchFinished.AddUObject(this, &USMenuJoinSessionWidget::OnSessionSearchComplete, SessionSearchRequest);
				SessionSubsystem->FindSessions(GetOwningPlayer(), SessionSearchRequest);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[USMenuJoinSessionWidget::StartSearch] - Failed to create session search"));
			}
		}
	}
}

void USMenuJoinSessionWidget::SetSpinnerDisplay(const bool bSpinnerState) const
{
	if(SpinnerDisplay)
	{
		SpinnerDisplay->SetVisibility(bSpinnerState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void USMenuJoinSessionWidget::OnRefreshList()
{
	if(!bSearchInProgress)
	{
		StartSearch();
	}
}

void USMenuJoinSessionWidget::OnSessionSearchComplete(bool bSucceeded, const FText& ErrorMessage,
	UCommonSession_SearchSessionRequest* SessionSearchRequest)
{
	bSearchInProgress = false;
	SetSpinnerDisplay(false);
	
	if(bSucceeded && SessionSearchRequest)
	{
		if(SessionSearchRequest->Results.Num() > 0)
		{
			ListView->SetListItems(SessionSearchRequest->Results);
			ListView->SetUserFocus(GetOwningPlayer());
			NoSessionsDisplay->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			NoSessionsDisplay->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		NoSessionsDisplay->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Error, TEXT("[USMenuJoinSessionWidget::OnSessionSearchComplete] - Failed session search"))
	}
}

void USMenuJoinSessionWidget::OnBackMenu()
{
	if(const UWorld* WorldContext = GetWorld())
	{
		if(UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContext))
		{
			RootLayout->FindAndRemoveWidgetFromLayer(this);
		}
	}
}
