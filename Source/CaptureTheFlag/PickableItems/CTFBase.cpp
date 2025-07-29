// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFBase.h"

#include "CaptureTheFlag/Components/CTFBaseArea.h"
#include "CaptureTheFlag/GameModes/MatchGameMode.h"
#include "CaptureTheFlag/GameStates/CTFGameState.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACTFBase::ACTFBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseAreaComponent = CreateDefaultSubobject<UCTFBaseArea>(TEXT("BaseArea"));
	RootComponent = BaseAreaComponent;

	PickUpComponent = CreateDefaultSubobject<UCTFPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->SetupAttachment(BaseAreaComponent);

	TeamColor = ETeamColor::None;
}

void ACTFBase::PickedUp(ACTFCharacter* InstigatorActor)
{
	Super::PickedUp(InstigatorActor);

	if (HasAuthority())
	{
		if (InstigatorActor->GetHasFlag() && TeamColor==InstigatorActor->GetPlayerTeam())
		{
			AMatchGameMode* MatchGameMode = Cast<AMatchGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (MatchGameMode)
			{
				MatchGameMode->TeamScored(InstigatorActor);
			}
		}
	}
}

// Called when the game starts or when spawned
void ACTFBase::BeginPlay()
{
	Super::BeginPlay();

	PickUpComponent->EnablePickup(true);
}


