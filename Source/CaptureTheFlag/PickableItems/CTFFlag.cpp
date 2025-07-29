// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFFlag.h"

#include "CaptureTheFlag/Components/CTFFlagComponent.h"
#include "CaptureTheFlag/Components/CTFPickUpComponent.h"


// Sets default values
ACTFFlag::ACTFFlag()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FlagComponent = CreateDefaultSubobject<UCTFFlagComponent>(TEXT("FlagComponent"));
	RootComponent = FlagComponent;
	FlagComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickUpComponent = CreateDefaultSubobject<UCTFPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->SetupAttachment(FlagComponent);

	bReplicates = true;
}

void ACTFFlag::PickedUp(ACTFCharacter* InstigatorActor)
{
	Super::PickedUp(InstigatorActor);

	if (HasAuthority())
	{
		FlagComponent->FlagWasCaught(InstigatorActor);
		PickUpComponent->EnablePickup(false);
		PickedUp_Multicast(InstigatorActor);
	}
}

void ACTFFlag::PickedUp_Multicast_Implementation(ACTFCharacter* InstigatorActor)
{
	if (!HasAuthority())
	{
		FlagComponent->FlagWasCaught(InstigatorActor);
		PickUpComponent->EnablePickup(false);
	}
}

// Called when the game starts or when spawned
void ACTFFlag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTFFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACTFFlag::FlagDropped_Implementation(FVector FlagLocation)
{
	FVector NewLocation = FVector(FlagLocation.X, FlagLocation.Y, GetActorLocation().Z);
	SetActorLocation(NewLocation);
	if (HasAuthority())
	{
		EnablePickableFlag();
	}
}

void ACTFFlag::EnablePickableFlag()
{
	if (HasAuthority())
	{
		FlagComponent->EnablePickableFlag();
		PickUpComponent->EnablePickup(true);
		EnablePickableFlag_Multicast();
	}
}

void ACTFFlag::EnablePickableFlag_Multicast_Implementation()
{
	if (!HasAuthority())
	{
		FlagComponent->EnablePickableFlag();
		PickUpComponent->EnablePickup(true);
	}
}

