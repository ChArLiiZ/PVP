// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include <functional>

#include "Kismet/KismetSystemLibrary.h"
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

void UCombatComponent::GetStamina(bool IsPercentage, float& amount)
{
	if (IsPercentage)
	{
		amount = CurrentStamina / MaxStamina;
	}
	else
	{
		amount = CurrentStamina;
	}
}

void UCombatComponent::OnRep_Stamina(float PreviousStamina)
{
	UE_LOG(LogTemp, Warning, TEXT("Before: %f, After: %f"), PreviousStamina, CurrentStamina);
	OnHealthChangedDelegate.Broadcast(PreviousStamina, CurrentStamina);
}



void UCombatComponent::SV_AddStamina_Implementation(float amount)
{
	float PreviousStamina = CurrentStamina;
	CurrentStamina += amount;
	if (CurrentStamina > MaxStamina)
		CurrentStamina = MaxStamina;
	if (CurrentStamina < 0)
		CurrentStamina = 0;

	
	OnRep_Health(PreviousStamina);
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


void UCombatComponent::TraceStart_Implementation(FTraceInfo TraceInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Trace start"), *TraceInfo.Start.ToString());
	
	
}

void UCombatComponent::TraceEnd_Implementation(FName SocketName)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Trace end"), *SocketName.ToString());
}

void UCombatComponent::Timer_Trace()
{
	//UE_LOG(LogTemp, Warning, TEXT("Attack Tracing"));
	TArray<FTraceInfo> TraceInfoArray;
	TraceInfos.GenerateValueArray(TraceInfoArray);
	for (TTuple<FName, FTraceInfo> TraceInfo : &TraceInfos)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Add(TraceInfo.Value.ObjectType);
		TArray<AActor*> ActorsIgnore;
		ActorsIgnore.Add(GetOwner());
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
			TraceInfo.Value.SKMesh->GetSocketLocation(TraceInfo.Value.Start),
			TraceInfo.Value.SKMesh->GetSocketLocation(TraceInfo.Value.End),
			TraceInfo.Value.Radius,
			ObjectTypesArray,
			false,
			ActorsIgnore,
			EDrawDebugTrace::ForDuration,
			HitResults,
			true
			);
		for (FHitResult HitResult : HitResults)
		{
			if (!TraceInfo.Value.HitActors.Contains(HitResult.GetActor()))
			{
				TraceInfo.Value.HitActors.AddUnique(HitResult.GetActor());
				for (AActor* Actors : TraceInfo.Value.HitActors)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *Actors->GetName());
				}
				//UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());
			}
			
		}
		
			
			
		
	}
	
	
	
}

