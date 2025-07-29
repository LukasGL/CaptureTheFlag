// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
//#include "InputMappingContext.h"
//#include "../GAS/GameplayAbilities/Input/CTFPlayerGameplayAbilitiesDataAsset.h"
#include "CTFCharacter.generated.h"

class UCTFAttributeSet;
enum class ETeamColor : uint8;
class UInputMappingContext;
class UCTFPlayerGameplayAbilitiesDataAsset;

UCLASS()
class CAPTURETHEFLAG_API ACTFCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ThirdPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** GAS */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	class UCTFAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	UCTFAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DefaultAttributesEffectClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class UCTFWeaponComponent* Weapon;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool bHasFlag = false;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool bHasWeapon = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool bIsDead = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	ETeamColor PlayerTeam;

public:
	// Sets default values for this character's properties
	ACTFCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void NotifyControllerChanged() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCTFPlayerGameplayAbilitiesDataAsset> PlayerGameplayAbilitiesDataAsset;

	void InitAbilitySystem();
	void OnAbilityInputPressed(int32 InputID);
	void OnAbilityInputReleased(int32 InputID);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void RespawnCharacter();

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable)
	void SetPlayerTeam(ETeamColor NewPlayerTeam) { PlayerTeam = NewPlayerTeam; }
	UFUNCTION(BlueprintCallable)
	ETeamColor GetPlayerTeam() { return PlayerTeam; }

	UFUNCTION(BlueprintCallable)
	void RevokeFlag();

	UFUNCTION(BlueprintCallable)
	void DropFlag(FVector CurrentLocation);

	UFUNCTION(Client, Reliable)
	void RevokeFlagOnClient();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable)
	void AttributesBackToDefault();

	void SetHasFlag(bool HasFlag) { bHasFlag = HasFlag; }

	UFUNCTION(BlueprintCallable)
	bool GetHasFlag() const { return bHasFlag; }

	void SetHasWeapon(bool HasWeapon) { bHasWeapon = HasWeapon; }

	UFUNCTION(BlueprintCallable)
	bool GetHasWeapon() const { return bHasWeapon; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable)
	void Die();
	
	void ActivateAbilityByTag(FGameplayTag AbilityTag);

	void SetWeapon(UCTFWeaponComponent* NewWeapon) { Weapon = NewWeapon; }
	UCTFWeaponComponent* GetWeapon() { return Weapon; }

	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	USkeletalMeshComponent* GetThirdPersonMesh() const { return ThirdPersonMesh; }

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent;	}
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	FORCEINLINE UCTFPlayerGameplayAbilitiesDataAsset* GetPlayerGameplayAbilitiesDataAsset() const { return PlayerGameplayAbilitiesDataAsset; }

	
};
