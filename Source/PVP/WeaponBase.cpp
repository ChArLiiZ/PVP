// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Net/UnrealNetwork.h"


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
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	
	
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

