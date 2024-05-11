// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "PVP/DataAssets/WeaponInfos.h"
#include "WeaponBase.generated.h"

class APVPCharacter;
enum EInputType : int;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpecialAttackChargeCompleted);

UCLASS()
class PVP_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWeaponInfos* WeaponInfo;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ChargeSeconds = 2;
	UPROPERTY(BlueprintReadOnly)
	bool bChargeComplete = false;
	
	UPROPERTY(BlueprintAssignable)
	FOnSpecialAttackChargeCompleted OnSpecialAttackChargeCompletedDelegate;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Mesh_Outline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* TrailComp;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn), Replicated)
	APVPCharacter* OwnerRef;
	

	UFUNCTION(BlueprintNativeEvent)
	void InitialSetup();

	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* BasicAttack(EInputType InputType, float ElapsedSeconds, float TriggeredSeconds);
	
	
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* Sprint(EInputType InputType, FVector InputVector);

	UFUNCTION(BlueprintCallable)
	virtual void Guard(EInputType InputType);

	UFUNCTION(BlueprintCallable)
	virtual void SpecialAttack(EInputType InputType, float ElapsedSeconds, float TriggeredSeconds);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Trail(bool IsStart, int32 Index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
