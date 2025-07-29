// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameplayAbility_Move.h"
#include "EnhancedInputComponent.h"
#include "CaptureTheFlag/Characters/CTFCharacter.h"

void UCTFGameplayAbility_Move::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!GetCharacterOwner()) return;
	
	// find out which way is forward
	const FRotator Rotation = GetCharacterOwner()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	GetCharacterOwner()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetCharacterOwner()->AddMovementInput(RightDirection, MovementVector.X);
}

void UCTFGameplayAbility_Move::OnTriggeredInputAction(const FInputActionValue& Value)
{
	Super::OnTriggeredInputAction(Value);
	
	Move(Value);
}

void UCTFGameplayAbility_Move::OnReleasedInputAction(const FInputActionValue& Value)
{
	Super::OnReleasedInputAction(Value);
}
