// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFPickableItemBase.h"
#include "GameFramework/Actor.h"
#include "CTFFlag.generated.h"

UCLASS()
class CAPTURETHEFLAG_API ACTFFlag : public ACTFPickableItemBase
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	class UCTFFlagComponent* FlagComponent;

public:
	// Sets default values for this actor's properties
	ACTFFlag();

	virtual void PickedUp(ACTFCharacter* InstigatorActor) override;

	UFUNCTION(NetMulticast, Reliable)
	void PickedUp_Multicast(ACTFCharacter* InstigatorActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void FlagDropped(FVector FlagLocation);

	UFUNCTION(BlueprintCallable)
	void EnablePickableFlag();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void EnablePickableFlag_Multicast();
};
