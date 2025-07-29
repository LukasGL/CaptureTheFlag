// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFFlagComponent.h"

#include "CaptureTheFlag/Characters/CTFCharacter.h"

// Sets default values for this component's properties
UCTFFlagComponent::UCTFFlagComponent()
{
	
}

void UCTFFlagComponent::InitializeComponent()
{
	Super::InitializeComponent();

	
}

void UCTFFlagComponent::FlagWasCaught(ACTFCharacter* Character)
{
	if (Character)
	{
		Character->SetHasFlag(true);
		SetVisibility(false);
	}
}

void UCTFFlagComponent::EnablePickableFlag()
{
	SetVisibility(true);
}
