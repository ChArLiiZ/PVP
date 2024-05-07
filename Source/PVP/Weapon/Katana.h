// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Katana.generated.h"

UCLASS()
class PVP_API AKatana : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKatana();


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SpecialAttack(EInputType InputType, float ElapsedSeconds, float TriggeredSeconds) override;

	
};
