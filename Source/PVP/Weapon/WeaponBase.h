// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVP/PVPCharacter.h"
#include "PVP/DataAssets/WeaponInfos.h"
#include "WeaponBase.generated.h"


UENUM(Blueprintable, BlueprintType)
enum EWeaponTypes
{
	Katana
};

UCLASS()
class PVP_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWeaponInfos WeaponInfo;
	
	UPROPERTY(Category="Attributes")
	TEnumAsByte<EWeaponTypes> WeaponType;
	UPROPERTY(Category="Attributes")
	FName SocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Mesh_Outline;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn), Replicated)
	APVPCharacter* OwnerRef;
	

	UFUNCTION(BlueprintNativeEvent)
	void InitialSetup();

	UFUNCTION()
	void LoadWeaponInfos();
	


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
