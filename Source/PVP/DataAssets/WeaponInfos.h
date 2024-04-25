// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PVP/Weapon/WeaponBase.h"
#include "WeaponInfos.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponAttributes
{
	GENERATED_BODY()

	EWeaponTypes WeaponType;
	FName SocketName;
	
	
};


UCLASS()
class PVP_API UWeaponInfos : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	FWeaponAttributes Attributes;

	
};
