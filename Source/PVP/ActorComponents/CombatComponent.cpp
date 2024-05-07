// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"


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

void UCombatComponent::OnRep_TagContainer_Implementation(FGameplayTagContainer Previous)
{
	
}


void UCombatComponent::SV_AddHealth_Implementation(float amount)
{
	float PreviousHealth = CurrentHealth;
	CurrentHealth += amount;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
	if (CurrentHealth < 0)
		CurrentHealth = 0;

	if (CurrentHealth <= 0)
		SetDead(true);
	

	
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
	
	OnStaminaChangedDelegate.Broadcast(PreviousStamina, CurrentStamina);
}

void UCombatComponent::SetDead(bool dead)
{
	this->bDead = dead;
	OnRep_Dead();
}

void UCombatComponent::OnRep_Dead_Implementation()
{
	if (bDead == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is Dead"), *GetOwner()->GetName())
		OnDeadDelegate.Broadcast();
	}
}


bool UCombatComponent::AddStamina(float amount)
{
	float PreviousStamina = CurrentStamina;
	if ((CurrentStamina + amount) < 0)
	{
		return false;
	}
	CurrentStamina += amount;
	if (CurrentStamina > MaxStamina)
		CurrentStamina = MaxStamina;
	
	OnRep_Stamina(PreviousStamina);
	return true;
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
	DOREPLIFETIME(UCombatComponent, CurrentStamina);
	DOREPLIFETIME(UCombatComponent, TagContainer);
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
	for (TTuple<FName, FTraceInfo> TraceInfo : TraceInfos)
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
			EDrawDebugTrace::None,        //Debug
			HitResults,
			true
			);
		for (FHitResult HitResult : HitResults)
		{
			if (!TraceInfos.Find(TraceInfo.Key)->HitActors.Contains(HitResult.GetActor()))
			{
				TraceInfos.Find(TraceInfo.Key)->HitActors.AddUnique(HitResult.GetActor());
				
				UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetName());
				if (UCombatComponent* TargetCombatComponent = Cast<UCombatComponent>
					(HitResult.GetActor()->GetComponentByClass(UCombatComponent::StaticClass())))
				{
					if (TargetCombatComponent->TagContainer.HasTag(GuardingTag))
					{
						TargetCombatComponent->OnGuardDelegate.Broadcast(this, HitResult, BaseDamage * TraceInfo.Value.DamageRatio, TraceInfo.Value.ImpactType);
						OnHitDelegate.Broadcast(HitResult, true, TraceInfo.Value.ImpactType);
					}
					else
					{
						TargetCombatComponent->SV_DealDamage(BaseDamage * TraceInfo.Value.DamageRatio,
														HitResult, this, TraceInfo.Value.ImpactType);
						OnHitDelegate.Broadcast(HitResult, false, TraceInfo.Value.ImpactType);
					}
					
				}
				
			}
		}
	}
}

void UCombatComponent::SV_DealDamage_Implementation(float amount, FHitResult HitResult, UCombatComponent* Source, EImpactTypes ImpactType)
{
	GetHitDelegate.Broadcast(Source, HitResult, amount, ImpactType);
	SV_AddHealth(amount * -1);
	UE_LOG(LogTemp, Warning, TEXT("%s Deal %f Damage To %s"), *Source->GetOwner()->GetName(), amount, *GetOwner()->GetName());
}

