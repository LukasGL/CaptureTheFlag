// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class ACTFFlag;
struct FTeamInfo;
enum class ETeamColor : uint8;
class ACTFCharacter;
class ACTFBase;
/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API AMatchGameMode : public AGameModeBase
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Setup)
	TSubclassOf<ACTFCharacter> ShooterDefaultClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Setup)
	TSubclassOf<ACTFFlag> FlagDefaultClass;

	

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Setup)
	TMap<ETeamColor, ACTFBase*> TeamColorToBaseMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Setup)
	APlayerStart* FlagSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Setup)
	ACTFFlag* FlagActor = nullptr;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	FTeamInfo CreateOrGetTeam(ETeamColor TeamColor);

	UFUNCTION(BlueprintCallable)
	void SpawnNewShooter(APlayerController* NewPlayer, ETeamColor TeamColor);
	
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SpawnFlag();

	UFUNCTION(BlueprintCallable, Category = Setup)
	bool CheckIfTeamWon();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void RestartMatch();

	FTimerHandle DelayHandle;
	
	UFUNCTION(BlueprintCallable, Category = Setup)
	void StartTimeGapTillNextMatch();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void FlagDropped(FVector FlagLocation);

	UFUNCTION(BlueprintCallable)
	void TeamScored(ACTFCharacter* Player);
	
	AMatchGameMode();
	
};
