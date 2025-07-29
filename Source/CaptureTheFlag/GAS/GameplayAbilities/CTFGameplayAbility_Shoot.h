// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFGameplayAbilityBase.h"
#include "CTFGameplayAbility_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFGameplayAbility_Shoot : public UCTFGameplayAbilityBase
{

	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
	class UCTFWeaponComponent* AttachedWeapon; 

	/** Called for movement input */
	void Shoot();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	
	virtual void OnTriggeredInputAction(const FInputActionValue& Value) override;
	virtual void OnReleasedInputAction(const FInputActionValue& Value) override;

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

};
