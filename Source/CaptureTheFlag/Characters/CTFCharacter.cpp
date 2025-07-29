// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CaptureTheFlag/GameModes/MatchGameMode.h"
#include "CaptureTheFlag/GameStates/CTFGameState.h"
#include "CaptureTheFlag/GAS/AbilitySystemComponent/CTFAbilitySystemComponent.h"
#include "CaptureTheFlag/GAS/AttributeSet/CTFAttributeSet.h"
#include "CaptureTheFlag/GAS/GameplayAbilities/CTFGameplayAbilityBase.h"
#include "CaptureTheFlag/GAS/GameplayAbilities/Input/CTFPlayerGameplayAbilitiesDataAsset.h"
#include "CaptureTheFlag/PickableItems/CTFBase.h"
#include "CaptureTheFlag/PlayerControllers/CTFShooterPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACTFCharacter::ACTFCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMesh->SetOwnerNoSee(true);
	ThirdPersonMesh->SetupAttachment(RootComponent);
	ThirdPersonMesh->bCastDynamicShadow = false;
	ThirdPersonMesh->CastShadow = false;
	ThirdPersonMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UCTFAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UCTFAttributeSet>(TEXT("AttributeSet"));

	if (!AbilitySystemComponent->DefaultStartingData.IsEmpty())
		AbilitySystemComponent->InitStats(UCTFAttributeSet::StaticClass(), AbilitySystemComponent->DefaultStartingData[0].DefaultStartingTable);

}

void ACTFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTFCharacter, bHasFlag);
	DOREPLIFETIME(ACTFCharacter, bIsDead);
	DOREPLIFETIME(ACTFCharacter, bHasWeapon);
}

// Called when the game starts or when spawned
void ACTFCharacter::BeginPlay()
{
	
	Super::BeginPlay();
	
	GetCharacterMovement()->AirControl = 1.f;
	
	InitAbilitySystem();
	
}

void ACTFCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (ACTFShooterPlayerController* PC = Cast<ACTFShooterPlayerController>(GetController()))
	{
		PC->SetupMappingContext();
	}
	
}

void ACTFCharacter::InitAbilitySystem()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent)
	{
		return;
	}
	
	if (PlayerGameplayAbilitiesDataAsset)
	{
		const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerGameplayAbilitiesDataAsset->GetInputAbilities();
		constexpr int32 AbilityLevel = 1;
  
		for (const auto& It : InputAbilities)
		{
			if (It.IsValid())
			{
				const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(It.GameplayAbilityClass, AbilityLevel, It.InputID);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
	}
}

void ACTFCharacter::OnAbilityInputPressed(int32 InputID)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->PressInputID(InputID);
	}
}

void ACTFCharacter::OnAbilityInputReleased(int32 InputID)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputReleased(InputID);
	}
}

// Called every frame
void ACTFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACTFCharacter::RespawnCharacter()
{
	if (ACTFGameState* GS = Cast<ACTFGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		if (ACTFBase* TeamBase = GS->GetTeamBase(PlayerTeam))
		{
			SetActorLocation(TeamBase->GetActorLocation());
			SetActorRotation(TeamBase->GetActorRotation());
		}
	}
}

void ACTFCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);
}

void ACTFCharacter::RevokeFlag()
{
	SetHasFlag(false);
}

void ACTFCharacter::DropFlag(FVector CurrentLocation)
{
	if (HasAuthority())
	{
		if (AMatchGameMode* GM = Cast<AMatchGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GM->FlagDropped(CurrentLocation);
		}
	}
}

void ACTFCharacter::ApplyDamage(float DamageAmount)
{
	if (!AbilitySystemComponent) return;

	if (DamageEffectClass)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DamageEffectClass, // TSubclassOf<UGameplayEffect>
			1.0f,
			ContextHandle
		);

		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), DamageAmount);

			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ACTFCharacter::AttributesBackToDefault()
{
	if (!AbilitySystemComponent) return;
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributesEffectClass, 1.0f, EffectContext);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
	//AbilitySystemComponent->AttributesBackToDefault();

	bIsDead = false;
}

void ACTFCharacter::Die()
{
	if (HasAuthority())
	{
		bIsDead = true;

		FGameplayTag DieTag = FGameplayTag::RequestGameplayTag(FName("Ability.Die"));
		ActivateAbilityByTag(DieTag);
	}
}

void ACTFCharacter::ActivateAbilityByTag(FGameplayTag AbilityTag)
{
	if (AbilitySystemComponent)
	{
        
		for (FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if (AbilitySpec.Ability && AbilitySpec.Ability->AbilityTags.HasTag(AbilityTag))
			{
				AbilitySystemComponent->TryActivateAbility(AbilitySpec.Handle);
				UE_LOG(LogTemp, Log, TEXT("Activating ability by tag: %s"), *AbilityTag.ToString());
				break;
			}
		}
	}
}

void ACTFCharacter::RevokeFlagOnClient_Implementation()
{
	RevokeFlag();
}

UAbilitySystemComponent* ACTFCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called to bind functionality to input
void ACTFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (PlayerGameplayAbilitiesDataAsset)
		{
			const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerGameplayAbilitiesDataAsset->GetInputAbilities();
			for (const auto& It : InputAbilities)
			{
				if (It.IsValid())
				{
					const UInputAction* InputAction = It.InputAction;
					const int32 InputID = It.InputID;
     
					EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ACTFCharacter::OnAbilityInputPressed, InputID);
					EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ACTFCharacter::OnAbilityInputReleased, InputID);
				}
			}
		}
	}

}

