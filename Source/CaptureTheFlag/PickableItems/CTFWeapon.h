// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFPickableItemBase.h"
#include "GameFramework/Actor.h"
#include "CTFWeapon.generated.h"

class UCTFPickUpComponent;
class UCTFWeaponComponent;

UCLASS()
class CAPTURETHEFLAG_API ACTFWeapon : public ACTFPickableItemBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon, meta = (AllowPrivateAccess = "true"))
	UCTFWeaponComponent* WeaponComponent;

public:
	// Sets default values for this actor's properties
	ACTFWeapon();

	virtual void PickedUp(ACTFCharacter* InstigatorActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
