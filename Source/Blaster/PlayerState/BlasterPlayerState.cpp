// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"

void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerState, Defeats);
	DOREPLIFETIME(ABlasterPlayerState, Team);
}

void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);

	LocalController = LocalController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : LocalController;

	if (LocalController)
	{
		LocalController->RefreshPlayerScore(this);
	}
}

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	LocalController = LocalController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : LocalController;

	if (LocalController)
	{
		LocalController->RefreshPlayerScore(this);
	}
}

void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;

	LocalController = LocalController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : LocalController;

	if (LocalController)
	{
		LocalController->RefreshPlayerScore(this);
	}
}

void ABlasterPlayerState::OnRep_Defeats()
{
	LocalController = LocalController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : LocalController;

	if (LocalController)
	{
		LocalController->RefreshPlayerScore(this);
	}
}

void ABlasterPlayerState::SetTeam(ETeam TeamToSet)
{
	Team = TeamToSet;

	ABlasterCharacter* BCharacter = Cast<ABlasterCharacter>(GetPawn());
	if (BCharacter)
	{
		BCharacter->SetTeamColor(Team);
	}

	LocalController = LocalController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : LocalController;

	if (LocalController)
	{
		LocalController->RefreshPlayerScore(this);
	}
}

void ABlasterPlayerState::OnRep_Team()
{
	ABlasterCharacter* BCharacter = Cast<ABlasterCharacter>(GetPawn());
	if (BCharacter)
	{
		BCharacter->SetTeamColor(Team);
	}

	LocalController = LocalController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : LocalController;

	if (LocalController)
	{
		LocalController->RefreshPlayerScore(this);
	}
}

