// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "AsyncTreeDifferences.h"
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

	SetReplicates(true);
}

void AWeaponBase::InitialSetup_Implementation()
{
	
	SKMesh->AttachToComponent(OwnerRef->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponInfo->Attributes.SocketName);
	
	SetOwner(OwnerRef->Owner);
	OwnerRef->CombatComponent->BaseDamage = WeaponInfo->Attributes.BaseDamage;
}


UAnimMontage* AWeaponBase::BasicAttack(EInputType InputType, float ElapsedSeconds, float TriggeredSeconds)
{
	int32 MontageIndex = 0;

	if (InputType != Started)
	{
		return nullptr;
	}
	
	if (OwnerRef->TagComponent->TagContainer.HasTag(WeaponInfo->Animations.BasicAttackingTag)
		|| OwnerRef->TagComponent->TagContainer.HasTag(WeaponInfo->Animations.SprintingTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Basic Attack"));
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
	UE_LOG(LogTemp, Warning, TEXT("Play Basic Attack %i"), MontageIndex);
	return WeaponInfo->Animations.BasicAttacks[MontageIndex];

	
}

UAnimMontage* AWeaponBase::Sprint(EInputType InputType, FVector InputVector)
{
	
	if (OwnerRef->GetCharacterMovement()->IsFalling() || OwnerRef->TagComponent->TagContainer.HasTag(WeaponInfo->Animations.SprintingTag))
	{
		return nullptr;
	}
	if (InputVector.Length() >= 0)
	{
		OwnerRef->SetActorRotation(UKismetMathLibrary::MakeRotFromX(InputVector));
	}
	
	return WeaponInfo->Animations.Sprint;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	InitialSetup();
	
	
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

