// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFGameplayAbilityBase.h"
#include "CTFGameplayAbility_Move.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFGameplayAbility_Move : public UCTFGameplayAbilityBase
{

	GENERATED_BODY()

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	virtual void OnTriggeredInputAction(const FInputActionValue& Value) override;
	virtual void OnReleasedInputAction(const FInputActionValue& Value) override;
};
