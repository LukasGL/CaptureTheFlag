// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFGameplayAbilityBase.h"
#include "CTFGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFGameplayAbility_Jump : public UCTFGameplayAbilityBase
{
	GENERATED_BODY()

protected:

	void AbilityJump();

	virtual void OnTriggeredInputAction(const FInputActionValue& Value) override;
	virtual void OnReleasedInputAction(const FInputActionValue& Value) override;
};
