// Fill out your copyright notice in the Description page of Project Settings.


#include "Katana.h"

#include "NiagaraScript.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PVP/PVPCharacter.h"


// Sets default values
AKatana::AKatana()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKatana::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKatana::SpecialAttack(EInputType InputType, float ElapsedSeconds, float TriggeredSeconds)
{
	Super::SpecialAttack(InputType, ElapsedSeconds, TriggeredSeconds);

	if (OwnerRef->GetCharacterMovement()->IsFalling() || OwnerRef->CombatComponent->TagContainer.HasAny(OwnerRef->CombatComponent->CantSpecialAttackTags))
	{
		return;
	}

	

	
	
	if (InputType == EInputType::Started)
	{
		float temp;
		OwnerRef->CombatComponent->GetStamina(false, temp);
		if (temp < WeaponInfo->Attributes.SpecialAttackStamina)
		{
			OwnerRef->CombatComponent->TagContainer.RemoveTag(OwnerRef->CombatComponent->ChargingTag);
			return;
		}
		OwnerRef->CombatComponent->TagContainer.AddTag(OwnerRef->CombatComponent->ChargingTag);
		bChargeComplete = false;
	}

	if (InputType == EInputType::Triggered)
	{
		if (TriggeredSeconds >= ChargeSeconds && bChargeComplete == false)
		{
			bChargeComplete = true;
			OnSpecialAttackChargeCompletedDelegate.Broadcast();
		}
	}

	if (InputType == EInputType::Completed || InputType == EInputType::Canceled)
	{
		
		if (bChargeComplete)
		{
			OwnerRef->CombatComponent->TagContainer.AddTag(OwnerRef->CombatComponent->ChargeEndTag);
		}
		OwnerRef->CombatComponent->TagContainer.RemoveTag(OwnerRef->CombatComponent->ChargingTag);
		bChargeComplete = false;
		
	}
	
	
}



// Called every frame
void AKatana::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

