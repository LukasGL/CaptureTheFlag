// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPickUpComponent.h"




// Sets default values for this component's properties
UCTFPickUpComponent::UCTFPickUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UCTFPickUpComponent::EnablePickup(bool bEnable)
{
	if (bEnable)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UCTFPickUpComponent::OnSphereBeginOverlap);
		return;
	}
	OnComponentBeginOverlap.RemoveAll(this);
}


// Called when the game starts
void UCTFPickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCTFPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ACTFCharacter* Character = Cast<ACTFCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		//EnablePickup(false);
	}
}

