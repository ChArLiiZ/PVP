// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVP/DataAssets/WeaponInfos.h"
#include "WeaponBase.generated.h"

class APVPCharacter;
enum EInputType : int;




UCLASS()
class PVP_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWeaponInfos* WeaponInfo;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Mesh_Outline;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn), Replicated)
	APVPCharacter* OwnerRef;
	

	UFUNCTION(BlueprintNativeEvent)
	void InitialSetup();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* BasicAttack(EInputType InputType, float ElapsedSeconds, float TriggeredSeconds);
	
	
	UFUNCTION(BlueprintCallable)
	UAnimMontage* Sprint(EInputType InputType, FVector InputVector);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
