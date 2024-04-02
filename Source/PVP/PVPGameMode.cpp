// Copyright Epic Games, Inc. All Rights Reserved.

#include "PVPGameMode.h"
#include "PVPCharacter.h"
#include "UObject/ConstructorHelpers.h"

APVPGameMode::APVPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
