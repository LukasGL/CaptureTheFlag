// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameplayAbility_Jump.h"

#include "CaptureTheFlag/Characters/CTFCharacter.h"

void UCTFGameplayAbility_Jump::AbilityJump()
{
	if (!GetCharacterOwner()) return;

	GetCharacterOwner()->Jump();
}

void UCTFGameplayAbility_Jump::OnTriggeredInputAction(const FInputActionValue& Value)
{
	Super::OnTriggeredInputAction(Value);

	AbilityJump();
}

void UCTFGameplayAbility_Jump::OnReleasedInputAction(const FInputActionValue& Value)
{
	Super::OnReleasedInputAction(Value);
	if (!GetCharacterOwner()) return;

	GetCharacterOwner()->StopJumping();
}
