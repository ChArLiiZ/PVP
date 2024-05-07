// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PVPCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AWeaponBase;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);


UENUM(BlueprintType, Blueprintable)
enum EInputType
{
	Triggered,
	Started,
	Ongoing,
	Canceled,
	Completed
};

UENUM(BlueprintType, Blueprintable)
enum EPlayerCharacter
{
	Yoll
};



UCLASS(config=Game)
class APVPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	APVPCharacter();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ActorComponents")
	UCombatComponent* CombatComponent;

	UPROPERTY(BlueprintReadWrite, Replicated)
	AWeaponBase* WeaponRef;

	

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LockTarget)
	AActor* LockTargetRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LockedIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LockDistance = 1000;

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_LockTarget(AActor* PreviousTarget);

	UFUNCTION(BlueprintCallable)
	void SetLockTarget(AActor* LockTarget);

	UFUNCTION(BlueprintNativeEvent)
	void LockToTarget();




protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

