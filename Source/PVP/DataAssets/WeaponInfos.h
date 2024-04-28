// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Engine/DataAsset.h"
#include "WeaponInfos.generated.h"


enum EPlayerCharacter : int;

UENUM(Blueprintable, BlueprintType)
enum EWeaponTypes
{
	Katana
};

UENUM(Blueprintable, BlueprintType)
enum EAttackType
{
	BasicAttack,
	HeavyAttack,
	Block
};


USTRUCT(BlueprintType, Blueprintable)
struct FWeaponAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EWeaponTypes> WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage;
};



USTRUCT(BlueprintType, Blueprintable)
struct FWeaponAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BasicAttackingTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> BasicAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SprintingTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Sprint;
};


UCLASS(BlueprintType, Blueprintable)
class PVP_API UWeaponInfos : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponAttributes Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponAnimations Animations;

	
	
};
