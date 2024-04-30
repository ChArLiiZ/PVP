// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Before, float, After);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedDelegate, float, Before, float, After);


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
	

	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnStaminaChangedDelegate OnStaminaChangedDelegate;



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
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
