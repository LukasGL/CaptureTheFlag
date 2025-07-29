// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFGameplayAbilityBase.h"
#include "CTFGameplayAbility_Die.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFGameplayAbility_Die : public UCTFGameplayAbilityBase
{
	GENERATED_BODY()

public:

	UCTFGameplayAbility_Die();

protected:

	UFUNCTION()
	void ActionsAfterDeath(ACTFCharacter* Character);

	UFUNCTION(Client, Reliable)
	void CharacterDied();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
