// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFPickableItemBase.h"
#include "CaptureTheFlag/Characters/CTFCharacter.h"
#include "CTFBase.generated.h"

enum class ETeamColor : uint8;

UCLASS()
class CAPTURETHEFLAG_API ACTFBase : public ACTFPickableItemBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Area, meta = (AllowPrivateAccess = "true"))
	class UCTFBaseArea* BaseAreaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Area, meta = (AllowPrivateAccess = "true"))
	ETeamColor TeamColor;

public:
	// Sets default values for this actor's properties
	ACTFBase();

	virtual void PickedUp(ACTFCharacter* InstigatorActor) override;

	UFUNCTION(BlueprintCallable)
	ETeamColor GetTeamColor() {return TeamColor;}
	UFUNCTION(BlueprintCallable)
	void SetTeamColor(ETeamColor NewTeamColor) {TeamColor = NewTeamColor;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
