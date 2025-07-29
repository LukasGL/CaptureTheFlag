// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CTFGameState.generated.h"

class ACTFCharacter;
class ACTFBase;

UENUM(BlueprintType)
enum class ETeamColor : uint8
{
	Blue,
	Green,
	Yellow,
	Red,
	None
};

USTRUCT(BlueprintType)
struct FTeamInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeamColor TeamColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ACTFCharacter*> TeamPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACTFBase* Base;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeamScored);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamWon, ETeamColor, TeamColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchRestarted);

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ACTFGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Teams)
	TMap<ETeamColor, FTeamInfo> TeamsPlaying;

	UPROPERTY(ReplicatedUsing=OnRep_BlueTeamInfo)
	FTeamInfo BlueTeamInfo;
	UPROPERTY(ReplicatedUsing=OnRep_GreenTeamInfo)
	FTeamInfo GreenTeamInfo;
	UPROPERTY(ReplicatedUsing=OnRep_YellowTeamInfo)
	FTeamInfo YellowTeamInfo;
	UPROPERTY(ReplicatedUsing=OnRep_RedTeamInfo)
	FTeamInfo RedTeamInfo;
	
public:

	ACTFGameState();

	UFUNCTION(BlueprintCallable)
	void RefreshTeamsInfoOnServer();

	UFUNCTION(NetMulticast, Reliable)
	void TeamWonNotifier(ETeamColor TeamColor);

	UFUNCTION(NetMulticast, Reliable)
	void RestartGameNotifier();

	UFUNCTION(BlueprintCallable)
	void TeamScored(ETeamColor TeamColor);

	UFUNCTION(BlueprintCallable)
	void SetTeamScore(ETeamColor TeamColor, float NewScore);

	UFUNCTION(BlueprintCallable)
	void AddNewTeamPlayer(ETeamColor TeamColor, ACTFCharacter* NewPlayer);

	UFUNCTION(NetMulticast, Reliable)
	void TeamScoredNotify(FTeamInfo TeamInfo);

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnTeamScored OnTeamModified;

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnTeamWon OnTeamWon;

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnMatchRestarted OnMatchRestarted;

	UFUNCTION(BlueprintCallable)
	void AddNewTeam(FTeamInfo NewTeam);
	
	UFUNCTION(BlueprintCallable)
	TMap<ETeamColor, FTeamInfo> GetTeamsPlaying() { return TeamsPlaying;	}

	UFUNCTION(BlueprintCallable)
	ACTFBase* GetTeamBase(ETeamColor TeamColor);

	UFUNCTION(BlueprintCallable)
	uint8 GetTeamScore(ETeamColor TeamColor);
	
	UFUNCTION(BlueprintCallable)
	bool IsPlayerOnTeam(ACTFCharacter* Character, ETeamColor TeamColor);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	FTeamInfo GetBlueTeamInfo() { return BlueTeamInfo; }
	UFUNCTION(BlueprintCallable)
	FTeamInfo GetGreenTeamInfo() { return GreenTeamInfo; }
	UFUNCTION(BlueprintCallable)
	FTeamInfo GetYellowTeamInfo() { return YellowTeamInfo; }
	UFUNCTION(BlueprintCallable)
	FTeamInfo GetRedTeamInfo() { return RedTeamInfo; }

	UFUNCTION()
	void OnRep_BlueTeamInfo();
	UFUNCTION()
	void OnRep_GreenTeamInfo();
	UFUNCTION()
	void OnRep_YellowTeamInfo();
	UFUNCTION()
	void OnRep_RedTeamInfo();
};


