// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PVPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PVP_API APVPPlayerController : public APlayerController
{
	GENERATED_BODY()



protected:
	virtual void OnNetCleanup(UNetConnection* Connection) override;
};
