// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Engine/DataAsset.h"
#include "WeaponInfos.generated.h"


enum EImpactTypes : int;
enum EPlayerCharacter : int;

UENUM(Blueprintable, BlueprintType)
enum EWeaponTypes
{
	Katana,
	Halberd,
	Gaunlet
};

UENUM(Blueprintable, BlueprintType)
enum EAttackType
{
	BasicAttack,
	HeavyAttack,
	Block
};

UENUM(Blueprintable, BlueprintType)
enum EEightDirection
{
	Back,
	Back_Left,
	Left,
	Front_Left,
	Front,
	Front_Right,
	Right,
	Back_Right,
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
	float BaseDamage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BasicAttackStamina = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpecialAttackStamina = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintStamina = 20;
	
};



USTRUCT(BlueprintType, Blueprintable)
struct FWeaponAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> BasicAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SpecialAttack;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EEightDirection>, UAnimMontage*> Stagger_Animations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* KnockDown_Animation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EImpactTypes>, UAnimMontage*> Guard_Hit_Animations;
	
};

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponSounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* OnHit_Light;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* OnHit_Heavy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* OnHit_Guarded_Light;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* OnHit_Guarded_Heavy;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponSounds Sounds;
		
	
};
