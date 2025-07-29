// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "CTFInGameUI.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void BindToHealth(UAbilitySystemComponent* ASC);

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void UpdateHealthUI(float NewHealth);
};
