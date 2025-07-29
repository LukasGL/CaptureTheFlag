// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFShooterPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "CaptureTheFlag/Characters/CTFCharacter.h"
#include "CaptureTheFlag/GameModes/MatchGameMode.h"
#include "Kismet/GameplayStatics.h"

void ACTFShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACTFShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}

void ACTFShooterPlayerController::SetupMappingContext()
{
	if (IsLocalController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACTFShooterPlayerController::InitInGameUI_Implementation()
{
	if (IsLocalController())
	{
		if (UUserWidget* MyWidget = CreateWidget<UUserWidget>(this, InGameUIClass))
		{
			bShowMouseCursor = false;
			MyWidget->AddToViewport();
		}
	}
}

void ACTFShooterPlayerController::InitTeamSelector_Implementation()
{
	if (IsLocalController())
	{
		if (UUserWidget* MyWidget = CreateWidget<UUserWidget>(this, TeamSelectorUIClass))
		{
			bShowMouseCursor = true;
			MyWidget->AddToViewport();
		}
	}
}

void ACTFShooterPlayerController::RequestJoiningTeam_Implementation(ETeamColor TeamColor)
{
	if (HasAuthority())
	{
		if (!GetPawn())
		{
			if (AMatchGameMode* GM = Cast<AMatchGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GM->SpawnNewShooter(this, TeamColor);
			}
		}
	}
}
