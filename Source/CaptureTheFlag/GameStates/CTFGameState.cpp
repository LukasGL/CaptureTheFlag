// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameState.h"

#include "Net/UnrealNetwork.h"

ACTFGameState::ACTFGameState()
{
	bReplicates = true;

	BlueTeamInfo = FTeamInfo();
	GreenTeamInfo = FTeamInfo();
	YellowTeamInfo = FTeamInfo();
	RedTeamInfo = FTeamInfo();
}

void ACTFGameState::RefreshTeamsInfoOnServer()
{
	if (TeamsPlaying.Contains(ETeamColor::Blue))
	{
		BlueTeamInfo = TeamsPlaying[ETeamColor::Blue];
	}
	if (TeamsPlaying.Contains(ETeamColor::Red))
	{
		RedTeamInfo = TeamsPlaying[ETeamColor::Red];
	}
	if (TeamsPlaying.Contains(ETeamColor::Green))
	{
		GreenTeamInfo = TeamsPlaying[ETeamColor::Green];
	}
	if (TeamsPlaying.Contains(ETeamColor::Yellow))
	{
		YellowTeamInfo = TeamsPlaying[ETeamColor::Yellow];
	}
	OnTeamModified.Broadcast();
}

void ACTFGameState::TeamWonNotifier_Implementation(ETeamColor TeamColor)
{
	OnTeamWon.Broadcast(TeamColor);
}

void ACTFGameState::TeamScored(ETeamColor TeamColor)
{
	TeamsPlaying[TeamColor].Score++;
	RefreshTeamsInfoOnServer();
}

void ACTFGameState::RestartGameNotifier_Implementation()
{
	OnMatchRestarted.Broadcast();
}

void ACTFGameState::SetTeamScore(ETeamColor TeamColor, float NewScore)
{
	TeamsPlaying[TeamColor].Score = NewScore;
}

void ACTFGameState::AddNewTeamPlayer(ETeamColor TeamColor, ACTFCharacter* NewPlayer)
{
	TeamsPlaying[TeamColor].TeamPlayers.Add(NewPlayer);
}

void ACTFGameState::TeamScoredNotify_Implementation(FTeamInfo TeamInfo)
{
	
}

void ACTFGameState::AddNewTeam(FTeamInfo NewTeam)
{
	TeamsPlaying.Add(NewTeam.TeamColor, NewTeam);
	RefreshTeamsInfoOnServer();
}

ACTFBase* ACTFGameState::GetTeamBase(ETeamColor TeamColor)
{
	if (TeamsPlaying.Contains(TeamColor))
	{
		return TeamsPlaying[TeamColor].Base;
	}
	return nullptr;
}

uint8 ACTFGameState::GetTeamScore(ETeamColor TeamColor)
{
	if (TeamsPlaying.Contains(TeamColor))
	{
		return TeamsPlaying[TeamColor].Score;
	}
	return 0;
}

bool ACTFGameState::IsPlayerOnTeam(ACTFCharacter* Character, ETeamColor TeamColor)
{
	return TeamsPlaying[TeamColor].TeamPlayers.Contains(Character);
}

void ACTFGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACTFGameState, BlueTeamInfo);
	DOREPLIFETIME(ACTFGameState, GreenTeamInfo);
	DOREPLIFETIME(ACTFGameState, YellowTeamInfo);
	DOREPLIFETIME(ACTFGameState, RedTeamInfo);
}

void ACTFGameState::OnRep_BlueTeamInfo()
{
	OnTeamModified.Broadcast();
}

void ACTFGameState::OnRep_GreenTeamInfo()
{
	OnTeamModified.Broadcast();
}

void ACTFGameState::OnRep_YellowTeamInfo()
{
	OnTeamModified.Broadcast();
}

void ACTFGameState::OnRep_RedTeamInfo()
{
	OnTeamModified.Broadcast();
}
