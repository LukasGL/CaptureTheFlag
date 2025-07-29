// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPickableItemBase.h"

#include "CaptureTheFlag/Components/CTFPickUpComponent.h"


// Sets default values
ACTFPickableItemBase::ACTFPickableItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACTFPickableItemBase::PickedUp(ACTFCharacter* InstigatorActor)
{
	CharacterOwner = InstigatorActor;
}

void ACTFPickableItemBase::EnableCapture(bool bEnable)
{
	if (bEnable)
	{
		FVector ActorLocation = GetActorLocation();
		if (CharacterOwner)
		{
			ActorLocation = CharacterOwner->GetActorLocation();
		}
		SetActorLocation(ActorLocation);
		CharacterOwner = nullptr;
		PickUpComponent->OnPickUp.AddDynamic(this, &ACTFPickableItemBase::PickedUp);
		
	}
	
}

// Called when the game starts or when spawned
void ACTFPickableItemBase::BeginPlay()
{
	Super::BeginPlay();

	EnableCapture(true);
	
}

// Called every frame
void ACTFPickableItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

