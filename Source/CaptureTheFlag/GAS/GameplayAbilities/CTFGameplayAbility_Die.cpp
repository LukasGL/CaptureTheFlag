// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameplayAbility_Die.h"

#include "CaptureTheFlag/Characters/CTFCharacter.h"

UCTFGameplayAbility_Die::UCTFGameplayAbility_Die()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;

	
}

void UCTFGameplayAbility_Die::ActionsAfterDeath(ACTFCharacter* Character)
{
	Character->RevokeFlag();
	Character->RespawnCharacter();
	Character->AttributesBackToDefault();
}

void UCTFGameplayAbility_Die::CharacterDied_Implementation()
{
	if (ACTFCharacter* Character = Cast<ACTFCharacter>(GetAvatarActorFromActorInfo()))
	{
		ActionsAfterDeath(Character);
	}
}

void UCTFGameplayAbility_Die::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthority(&ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	if (ACTFCharacter* Character = Cast<ACTFCharacter>(ActorInfo->AvatarActor))
	{
		if (Character->GetHasFlag())
		{
			Character->DropFlag(Character->GetActorLocation());
		}
		CharacterDied();
		ActionsAfterDeath(Character);
	}

	CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}
