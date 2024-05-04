// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UTagContainerComponent;

UENUM(Blueprintable, BlueprintType)
enum EImpactTypes
{
	None,
	Stagger,
	KnockDown
};

USTRUCT(Blueprintable, BlueprintType)
struct FTraceInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle TimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> HitActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EImpactTypes> ImpactType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KnockBackLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName End;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EObjectTypeQuery> ObjectType;
	
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Before, float, After);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedDelegate, float, Before, float, After);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FGetHitDelegate, UCombatComponent*, Source, FHitResult, HitResult, float, Damage, EImpactTypes, ImpactType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGuardDelegate, UCombatComponent*, Source, FHitResult, HitResult, EImpactTypes, ImpactType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitDelegate, FHitResult, HitResult, bool, IsGuarded);






UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class PVP_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();

	UPROPERTY(EditAnywhere, Category="Attributes", ReplicatedUsing=OnRep_Health)
	float CurrentHealth = 100;
	UPROPERTY(EditAnywhere, Category="Attributes")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category="Attributes", ReplicatedUsing=OnRep_Stamina)
	float CurrentStamina = 100;
	UPROPERTY(EditAnywhere, Category="Attributes")
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, Category="Attributes")
	float BaseDamage = 10;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FTraceInfo> TraceInfos;

	//Tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags", Replicated)
	FGameplayTagContainer TagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTag BasicAttackingTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTagContainer CantBasicAttackTags;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTag SprintingTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTagContainer CantSprintTags;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTag GuardingTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTagContainer CantGuardTags;
	

	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnStaminaChangedDelegate OnStaminaChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FGetHitDelegate GetHitDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnGuardDelegate OnGuardDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnHitDelegate OnHitDelegate;



	UFUNCTION(BlueprintCallable, Category="Attributes", Server, Reliable)
	void SV_AddHealth(float amount);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void GetHealth(bool IsPercentage, float& amount);

	UFUNCTION()
	void OnRep_Health(float PreviousHealth);


	UFUNCTION(BlueprintCallable, Category="Attributes", Server, Reliable)
	void SV_AddStamina(float amount);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	void GetStamina(bool IsPercentage, float& amount);

	UFUNCTION()
	void OnRep_Stamina(float PreviousHealth);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat")
	void TraceStart(FTraceInfo TraceInfo);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat")
	void TraceEnd(FName SocketName);
	
	UFUNCTION()
	void Timer_Trace();

	UFUNCTION(BlueprintCallable, Category="Combat", Server, Reliable)
	void SV_DealDamage(float amount, FHitResult HitResult, UCombatComponent* Source, EImpactTypes ImpactType);
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
