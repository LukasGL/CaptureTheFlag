// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTFPickableItemBase.generated.h"

class ACTFCharacter;
class UCTFPickUpComponent;

UCLASS()
class CAPTURETHEFLAG_API ACTFPickableItemBase : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pickable, meta = (AllowPrivateAccess = "true"))
	UCTFPickUpComponent* PickUpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pickable, meta = (AllowPrivateAccess = "true"))
	bool bCaptured = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Owner, meta = (AllowPrivateAccess = "true"))
	ACTFCharacter* CharacterOwner;

public:
	// Sets default values for this actor's properties
	ACTFPickableItemBase();

	UFUNCTION(BlueprintImplementableEvent)
	void WasSuccessfullyPickedUp(bool bValue);

	UFUNCTION()
	virtual void PickedUp(ACTFCharacter* InstigatorActor);

	UFUNCTION()
	void EnableCapture(bool bEnable);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
