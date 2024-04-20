// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PVPGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class PVP_API UPVPGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="EOS Functions")
	FString GetPlayerUserName();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="EOS Functions")
	bool IsPlayerLoggedIn();
	
	void LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess,
							const FUniqueNetId& UserID, const FString& Error);
};
