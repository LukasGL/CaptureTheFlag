// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CTFFlagComponent.generated.h"

class ACTFCharacter;
/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFFlagComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	UCTFFlagComponent();

	virtual void InitializeComponent() override;

	UFUNCTION(BlueprintCallable, Category=Flag)
	void FlagWasCaught(ACTFCharacter* Character);

	UFUNCTION(BlueprintCallable, Category=Flag)
	void EnablePickableFlag();
};
