// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "CaptureTheFlag/Characters/CTFCharacter.h"
#include "Net/UnrealNetwork.h"

UCTFAttributeSet::UCTFAttributeSet()
{
}

void UCTFAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	OnAttributeChanged.Broadcast();
}

void UCTFAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCTFAttributeSet, Health, OldHealth);
}

void UCTFAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
}

void UCTFAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	PostGameplayEffectExecute_Health(Data);
}

void UCTFAttributeSet::PostGameplayEffectExecute_Health(const FGameplayEffectModCallbackData& Data)
{
	// Verificar si el cambio fue en Health
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		float NewHealth = GetHealth();

		// Clamp por seguridad
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

		// Chequear si está muerto
		if (NewHealth <= 0.0f)
		{
			AActor* Owner = GetOwningActor();
			ACTFCharacter* Character = Cast<ACTFCharacter>(Owner);
			if (Character && !Character->IsDead())
			{
				Character->Die();
			}
		}
	}
}

void UCTFAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCTFAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCTFAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}
