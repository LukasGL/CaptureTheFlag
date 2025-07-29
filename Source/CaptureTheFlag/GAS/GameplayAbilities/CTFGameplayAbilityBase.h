// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CTFGameplayAbilityBase.generated.h"

class ACTFCharacter;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bCancelAbilityOnInputReleased;

protected:
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	ACTFCharacter * CharacterOwner;

protected:
	TArray<uint32> TriggeredEventHandles;

	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
public:
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~ End UGameplayAbility Interface

protected:
	virtual void OnTriggeredInputAction(const FInputActionValue& Value);
	virtual void OnReleasedInputAction(const FInputActionValue& Value);

public:
	ACTFCharacter* GetCharacterOwner() const
	{
		return CharacterOwner;
	}

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability"	)
	void BP_OnTriggeredInputAction(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability"	)
	void BP_OnReleasedInputAction(const FInputActionValue& Value);
};
