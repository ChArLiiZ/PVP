// Fill out your copyright notice in the Description page of Project Settings.


#include "PVPPlayerController.h"

#include "PVPGameInstance.h"

void APVPPlayerController::OnNetCleanup(UNetConnection* Connection)
{
	Super::OnNetCleanup(Connection);
}
