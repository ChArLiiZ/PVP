// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Before, float, After);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
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

	UPROPERTY(EditAnywhere, Category="Attributes")
	float CurrentStamina = 100;
	UPROPERTY(EditAnywhere, Category="Attributes")
	float MaxStamina = 100;

	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChangedDelegate;



	UFUNCTION(BlueprintCallable, Category="Attributes", Server, Reliable)
	void SV_AddHealth(float amount);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void GetHealth(bool IsPercentage, float& amount);

	UFUNCTION()
	void OnRep_Health(float PreviousHealth);
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
