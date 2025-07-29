// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFWeapon.h"

#include "CaptureTheFlag/Components/CTFPickUpComponent.h"
#include "CaptureTheFlag/Components/CTFWeaponComponent.h"


// Sets default values
ACTFWeapon::ACTFWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	WeaponComponent = CreateDefaultSubobject<UCTFWeaponComponent>(TEXT("WeaponComponent"));
	RootComponent = WeaponComponent;

	PickUpComponent = CreateDefaultSubobject<UCTFPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->SetupAttachment(WeaponComponent);
}

void ACTFWeapon::PickedUp(ACTFCharacter* InstigatorActor)
{
	Super::PickedUp(InstigatorActor);
	
	
	if (InstigatorActor && !InstigatorActor->GetHasWeapon() && WeaponComponent->AttachWeapon(InstigatorActor))
	{
		WasSuccessfullyPickedUp(true);
		InstigatorActor->SetHasWeapon(true);
		PickUpComponent->EnablePickup(false);
		EnableCapture(false);
	}
	
}

// Called when the game starts or when spawned
void ACTFWeapon::BeginPlay()
{
	Super::BeginPlay();

	PickUpComponent->EnablePickup(true);
}

// Called every frame
void ACTFWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

