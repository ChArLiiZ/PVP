// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
	
	// ...
}


void UCombatComponent::SV_AddHealth_Implementation(float amount)
{
	float PreviousHealth = CurrentHealth;
	CurrentHealth += amount;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
	if (CurrentHealth < 0)
		CurrentHealth = 0;

	
	OnRep_Health(PreviousHealth);
}

void UCombatComponent::GetHealth(bool IsPercentage, float& amount)
{
	if (IsPercentage)
	{
		amount = CurrentHealth / MaxHealth;
	}
	else
	{
		amount = CurrentHealth;
	}
}

void UCombatComponent::OnRep_Health(float PreviousHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("Before: %f, After: %f"), PreviousHealth, CurrentHealth);
	OnHealthChangedDelegate.Broadcast(PreviousHealth, CurrentHealth);
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, CurrentHealth);
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

