// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CTFShooterPlayerController.generated.h"

enum class ETeamColor : uint8;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ACTFShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:

	
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;


public:

	UFUNCTION(BlueprintCallable)
	void SetupMappingContext();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	TSubclassOf<UUserWidget> TeamSelectorUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	TSubclassOf<UUserWidget> InGameUIClass;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void RequestJoiningTeam(ETeamColor TeamColor);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void InitTeamSelector();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void InitInGameUI();
};
