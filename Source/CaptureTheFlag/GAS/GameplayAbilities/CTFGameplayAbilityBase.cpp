// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameplayAbilityBase.h"

#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputComponent.h"
#include "CaptureTheFlag/Characters/CTFCharacter.h"
#include "Input/CTFPlayerGameplayAbilitiesDataAsset.h"

UCTFGameplayAbilityBase::UCTFGameplayAbilityBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , bCancelAbilityOnInputReleased(true)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UCTFGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CharacterOwner = Cast<ACTFCharacter>(GetAvatarActorFromActorInfo());
	
	bool bSuccess = false;

	if (const ACTFCharacter* PlayerCharacter = Cast<ACTFCharacter>(GetAvatarActorFromActorInfo()))
	{
		EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerCharacter->InputComponent);
		if (EnhancedInputComponent)
		{
			if (const UCTFPlayerGameplayAbilitiesDataAsset* PlayerGameplayAbilitiesDataAsset = PlayerCharacter->
				GetPlayerGameplayAbilitiesDataAsset())
			{
				const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerGameplayAbilitiesDataAsset->
					GetInputAbilities();
				for (const auto& It : InputAbilities)
				{
					if (It.IsValid() && It.GameplayAbilityClass == GetClass())
					{
						const FEnhancedInputActionEventBinding& TriggeredEventBinding = EnhancedInputComponent->
							BindAction(It.InputAction, ETriggerEvent::Triggered, this,
							           &UCTFGameplayAbilityBase::OnTriggeredInputAction);
						
						const uint32 TriggeredEventHandle = TriggeredEventBinding.GetHandle();

						const FEnhancedInputActionEventBinding& ReleasedEventBinding = EnhancedInputComponent->
							BindAction(It.InputAction, ETriggerEvent::Completed, this,
									   &UCTFGameplayAbilityBase::OnReleasedInputAction);

						const uint32 ReleasedEventHandle = ReleasedEventBinding.GetHandle();

						TriggeredEventHandles.AddUnique(TriggeredEventHandle);
						TriggeredEventHandles.AddUnique(ReleasedEventHandle);

						bSuccess = true;
					}
				}
			}
		}
	}

	if (bSuccess)
	{
		// You can skip committing ability here if the ability was committed in the parent class
		CommitAbility(Handle, ActorInfo, ActivationInfo);
	}
	else
	{
		constexpr bool bReplicateCancelAbility = true;
		CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	}
}

void UCTFGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                         bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (EnhancedInputComponent)
	{
		for (const uint32 EventHandle : TriggeredEventHandles)
		{
			EnhancedInputComponent->RemoveBindingByHandle(EventHandle);
		}

		EnhancedInputComponent = nullptr;
	}

	TriggeredEventHandles.Reset();
}

void UCTFGameplayAbilityBase::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (bCancelAbilityOnInputReleased)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UCTFGameplayAbilityBase::OnTriggeredInputAction(const FInputActionValue& Value)
{
	BP_OnTriggeredInputAction(Value);
}

void UCTFGameplayAbilityBase::OnReleasedInputAction(const FInputActionValue& Value)
{
	BP_OnReleasedInputAction(Value);
}

void UCTFGameplayAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
}

bool UCTFGameplayAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                 const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCTFGameplayAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}
