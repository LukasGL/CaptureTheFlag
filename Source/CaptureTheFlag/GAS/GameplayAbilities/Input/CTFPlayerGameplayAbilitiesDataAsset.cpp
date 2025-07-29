// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPlayerGameplayAbilitiesDataAsset.h"

UCTFPlayerGameplayAbilitiesDataAsset::UCTFPlayerGameplayAbilitiesDataAsset(const FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer)
{
}

const TSet<FGameplayInputAbilityInfo>& UCTFPlayerGameplayAbilitiesDataAsset::GetInputAbilities() const
{
	return InputAbilities;
}

#if WITH_EDITOR
void UCTFPlayerGameplayAbilitiesDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FProperty* Property = PropertyChangedEvent.Property;
	if (Property && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCTFPlayerGameplayAbilitiesDataAsset, InputAbilities) && !InputAbilities.IsEmpty())
	{
		TArray<FGameplayInputAbilityInfo> InputAbilitiesArray = InputAbilities.Array();

		for (int32 i = InputAbilitiesArray.Num() - 1; i > 0; --i)
		{
			const int32 PrevIndex = i - 1;
			if (InputAbilitiesArray[i] == InputAbilitiesArray[PrevIndex])
			{
				InputAbilitiesArray.RemoveAtSwap(i);
			}
		}
  
		InputAbilities.Reset();
  
		for (int32 i = 0; i < InputAbilitiesArray.Num(); ++i)
		{
			FGameplayInputAbilityInfo& Info = InputAbilitiesArray[i];
			Info.InputID = i;
			InputAbilities.Add(Info);
		}
	}
}
#endif