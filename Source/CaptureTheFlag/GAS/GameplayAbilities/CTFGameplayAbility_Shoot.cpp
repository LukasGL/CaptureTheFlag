// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameplayAbility_Shoot.h"

#include "CaptureTheFlag/Characters/CTFCharacter.h"
#include "CaptureTheFlag/Components/CTFWeaponComponent.h"

void UCTFGameplayAbility_Shoot::Shoot()
{
	if (GetCharacterOwner() && GetCharacterOwner()->GetWeapon())
	{
		GetCharacterOwner()->GetWeapon()->Fire();
	}
}

void UCTFGameplayAbility_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Shoot();

	CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}

void UCTFGameplayAbility_Shoot::OnTriggeredInputAction(const FInputActionValue& Value)
{
	Super::OnTriggeredInputAction(Value);
}

void UCTFGameplayAbility_Shoot::OnReleasedInputAction(const FInputActionValue& Value)
{
	Super::OnReleasedInputAction(Value);
}

bool UCTFGameplayAbility_Shoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ACTFCharacter* AvatarActor = Cast<ACTFCharacter>(ActorInfo->AvatarActor);
	if (!AvatarActor || !AvatarActor->GetWeapon())
	{
		return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
