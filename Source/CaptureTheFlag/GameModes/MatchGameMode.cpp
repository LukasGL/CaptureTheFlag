// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"

#include "Blueprint/UserWidget.h"
#include "CaptureTheFlag/Characters/CTFCharacter.h"
#include "CaptureTheFlag/GameStates/CTFGameState.h"
#include "CaptureTheFlag/GAS/AttributeSet/CTFAttributeSet.h"
#include "CaptureTheFlag/PickableItems/CTFBase.h"
#include "CaptureTheFlag/PickableItems/CTFFlag.h"
#include "CaptureTheFlag/PlayerControllers/CTFShooterPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AMatchGameMode::AMatchGameMode()
	: Super()
{
	
}

void AMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ACTFShooterPlayerController* PC = Cast<ACTFShooterPlayerController>(NewPlayer))
	{
		PC->InitTeamSelector();
	}
	
}

FTeamInfo AMatchGameMode::CreateOrGetTeam(ETeamColor TeamColor)
{
	ACTFGameState* MatchGameState = Cast<ACTFGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (MatchGameState)
	{
		if (MatchGameState->GetTeamsPlaying().Contains(TeamColor))
		{
			return MatchGameState->GetTeamsPlaying()[TeamColor];
		}

		FTeamInfo NewTeam;
		NewTeam.Base = TeamColorToBaseMap[TeamColor];
		NewTeam.Base->SetTeamColor(TeamColor);
		NewTeam.Score = 0.f;
		NewTeam.TeamColor = TeamColor;
		MatchGameState->AddNewTeam(NewTeam);
		return NewTeam;
	}
	return FTeamInfo();
}

void AMatchGameMode::SpawnNewShooter(APlayerController* NewPlayer, ETeamColor TeamColor)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FTeamInfo Team = CreateOrGetTeam(TeamColor);
		ACTFBase* TeamBase = Team.Base;
		
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
		// Spawn the projectile at the muzzle
		if (ACTFCharacter* NewCharacter = World->SpawnActor<ACTFCharacter>(ShooterDefaultClass, TeamBase->GetActorLocation(), TeamBase->GetActorRotation(), ActorSpawnParams))
		{
			NewCharacter->SetPlayerTeam(TeamColor);
			NewCharacter->AttributesBackToDefault();
			if (ACTFGameState* MatchGameState = Cast<ACTFGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				MatchGameState->AddNewTeamPlayer(TeamColor, NewCharacter);
			}
			NewPlayer->Possess(NewCharacter);
			if (ACTFShooterPlayerController* PC = Cast<ACTFShooterPlayerController>(NewPlayer))
			{
				PC->InitInGameUI();
			}
		}
	}
	
	
}

void AMatchGameMode::SpawnFlag()
{
	if (FlagSpawn)
	{
		if (!FlagActor)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				FlagActor = World->SpawnActor<ACTFFlag>(FlagDefaultClass, FlagSpawn->GetActorLocation(), FlagSpawn->GetActorRotation(), ActorSpawnParams);
			}
		}
		FlagActor->FlagDropped(FlagSpawn->GetActorLocation());
	}
	
}

bool AMatchGameMode::CheckIfTeamWon()
{
	if (ACTFGameState* MatchGameState = Cast<ACTFGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		for (TTuple<ETeamColor, FTeamInfo> Team : MatchGameState->GetTeamsPlaying())
		{
			if (Team.Value.Score >= 3.f)
			{
				MatchGameState->TeamWonNotifier(Team.Key);
				StartTimeGapTillNextMatch();
				return true;
			}
		}
	}
	return false;
}

void AMatchGameMode::RestartMatch()
{
	if (ACTFGameState* MatchGameState = Cast<ACTFGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		for (TTuple<ETeamColor, FTeamInfo>& Team : MatchGameState->GetTeamsPlaying())
		{
			MatchGameState->SetTeamScore(Team.Key, 0.f);
			for (ACTFCharacter* Character : Team.Value.TeamPlayers)
			{
				FGameplayAttribute MaxHealthAttribute = UCTFAttributeSet::GetHealthAttribute();
				Character->ApplyDamage(Character->GetAbilitySystemComponent()->GetNumericAttribute(MaxHealthAttribute)*-1.f);
				//Character->RespawnCharacter();
			}
		}
		SpawnFlag();
		MatchGameState->RefreshTeamsInfoOnServer();
		MatchGameState->RestartGameNotifier();
	}
}

void AMatchGameMode::StartTimeGapTillNextMatch()
{
	GetWorld()->GetTimerManager().SetTimer(
		DelayHandle,
		this,
		&AMatchGameMode::RestartMatch,
		3.0f, // Tiempo en segundos
		false // No loop
	);
}

void AMatchGameMode::FlagDropped(FVector FlagLocation)
{
	FlagActor->FlagDropped(FlagLocation);
}

void AMatchGameMode::TeamScored(ACTFCharacter* Player)
{
	ACTFGameState* MatchGameState = Cast<ACTFGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (MatchGameState)
	{
		MatchGameState->TeamScored(Player->GetPlayerTeam());
		if (!Player->IsLocallyControlled())
		{
			Player->RevokeFlagOnClient();
		}
		Player->RevokeFlag();

		MatchGameState->RefreshTeamsInfoOnServer();

		UE_LOG(LogTemp, Warning, TEXT("Team Scored"));
		
		if (CheckIfTeamWon())
			return;

		SpawnFlag();
		
		
	}
}
