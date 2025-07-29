// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFBaseArea.h"


// Sets default values for this component's properties
UCTFBaseArea::UCTFBaseArea()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SphereRadius = 64.f;
	// ...
}


// Called when the game starts
void UCTFBaseArea::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCTFBaseArea::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

