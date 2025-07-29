// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFInGameUI.h"

#include "CaptureTheFlag/GAS/AttributeSet/CTFAttributeSet.h"

void UCTFInGameUI::BindToHealth(UAbilitySystemComponent* ASC)
{
	if (!ASC) return;

	ASC->GetGameplayAttributeValueChangeDelegate(UCTFAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UCTFInGameUI::OnHealthChanged);

	const float CurrentHealth = ASC->GetNumericAttribute(UCTFAttributeSet::GetHealthAttribute());
	UpdateHealthUI(CurrentHealth);
}

void UCTFInGameUI::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthUI(Data.NewValue);
}
