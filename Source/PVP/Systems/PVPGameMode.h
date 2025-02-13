// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PVPGameMode.generated.h"

UCLASS(minimalapi)
class APVPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APVPGameMode();


	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};



