// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameplayAbility_Look.h"

#include "InputActionValue.h"
#include "CaptureTheFlag/Characters/CTFCharacter.h"

void UCTFGameplayAbility_Look::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (ACTFCharacter* Character = Cast<ACTFCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->AddControllerYawInput(LookAxisVector.X);
		Character->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UCTFGameplayAbility_Look::OnTriggeredInputAction(const FInputActionValue& Value)
{
	Super::OnTriggeredInputAction(Value);

	Look(Value);
}

void UCTFGameplayAbility_Look::OnReleasedInputAction(const FInputActionValue& Value)
{
	Super::OnReleasedInputAction(Value);
}
