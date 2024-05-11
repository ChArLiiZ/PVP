// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PVP/PVPCharacter.h"
#include "PVP/DataAssets/WeaponInfos.h"



// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMesh"));
	RootComponent = SKMesh;
	Mesh_Outline = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_Outline"));
	Mesh_Outline->SetupAttachment(SKMesh);
	Mesh_Outline->SetLeaderPoseComponent(SKMesh, true, false);

	TrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	TrailComp->SetupAttachment(SKMesh);
	TrailComp->SetAutoActivate(false);

	bReplicates = true;
}

void AWeaponBase::InitialSetup_Implementation()
{
	
	SKMesh->AttachToComponent(OwnerRef->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponInfo->Attributes.SocketName);
	
	SetOwner(OwnerRef->Owner);
	OwnerRef->CombatComponent->BaseDamage = WeaponInfo->Attributes.BaseDamage;
	OwnerRef->GetMesh()->SetAnimInstanceClass(WeaponInfo->Animations.AnimBP);
}


void AWeaponBase::AdditionalSetup_Implementation(USkeletalMeshComponent* SkeletalMesh)
{
}

UAnimMontage* AWeaponBase::BasicAttack(EInputType InputType, float ElapsedSeconds, float TriggeredSeconds)
{
	

	if (InputType != Started)
	{
		return nullptr;
	}

	

	
	
	int32 MontageIndex = 0;
	
	if (OwnerRef->GetCharacterMovement()->IsFalling() ||
		OwnerRef->CombatComponent->TagContainer.HasAny(OwnerRef->CombatComponent->CantBasicAttackTags))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Can't Basic Attack"));
		return nullptr;
	}
	
	if (!OwnerRef->CombatComponent->AddStamina(WeaponInfo->Attributes.BasicAttackStamina * -1))
	{
		return nullptr;
	}
	
	for (UAnimMontage* PlayingMontage : WeaponInfo->Animations.BasicAttacks)
	{
		if (OwnerRef->GetCurrentMontage() == PlayingMontage)
		{
			MontageIndex = WeaponInfo->Animations.BasicAttacks.FindLast(PlayingMontage) + 1;
			break;
		}
	}

	if (MontageIndex >= WeaponInfo->Animations.BasicAttacks.Num())
	{
		MontageIndex = 0;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Play Basic Attack %i"), MontageIndex);
	return WeaponInfo->Animations.BasicAttacks[MontageIndex];

	
}

UAnimMontage* AWeaponBase::Sprint(EInputType InputType, FVector InputVector)
{
	

	
	if (OwnerRef->GetCharacterMovement()->IsFalling() ||
		OwnerRef->CombatComponent->TagContainer.HasAny(OwnerRef->CombatComponent->CantSprintTags))
	{
		return nullptr;
	}
	
	if (!OwnerRef->CombatComponent->AddStamina(WeaponInfo->Attributes.SprintStamina * -1))
	{
		return nullptr;
	}
	
	if (InputVector.Length() >= 0)
	{
		OwnerRef->SetActorRotation(UKismetMathLibrary::MakeRotFromX(InputVector));
	}
	
	return WeaponInfo->Animations.Sprint;
}

void AWeaponBase::Guard(EInputType InputType)
{
	if (InputType == EInputType::Started)
	{
		float Stamina;
		OwnerRef->CombatComponent->GetStamina(false, Stamina);
		if (Stamina <= 0)
		{
			return;
		}
		if (OwnerRef->GetCharacterMovement()->IsFalling() ||
			OwnerRef->CombatComponent->TagContainer.HasAny(OwnerRef->CombatComponent->CantGuardTags))
		{
			OwnerRef->CombatComponent->TagContainer.RemoveTag(OwnerRef->CombatComponent->GuardingTag);
			return;
		}
		OwnerRef->CombatComponent->TagContainer.AddTag(OwnerRef->CombatComponent->GuardingTag);
	}
	else if (InputType == EInputType::Completed)
	{
		OwnerRef->CombatComponent->TagContainer.RemoveTag(OwnerRef->CombatComponent->GuardingTag);
	}
	 
}

void AWeaponBase::SpecialAttack(EInputType InputType, float ElapsedSeconds, float TriggeredSeconds)
{
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

void AWeaponBase::Trail_Implementation(bool IsStart, int32 Index)
{
	if (IsStart)
	{
		TrailComp->Activate(false);
	}
	else
	{
		TrailComp->Deactivate();
	}
}


// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	if (OwnerRef)
	{
		InitialSetup();
	}
	
	
	
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, OwnerRef);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

