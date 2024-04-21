// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PVPGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class PVP_API UPVPGameInstance : public UGameInstance
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="EOS Functions")
	FString GetPlayerUserName();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="EOS Functions")
	bool IsPlayerLoggedIn();
	
	

	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void CreateEOSSession(bool bIsLan, int32 NumOfPublicConnections);

	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void FindSessions(int32 MaxSearchResults, TArray<FBlueprintSessionResult>& BlueprintSessionResults);
	
	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void JoinSession(int32 index);

	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void DestroySession();

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EOS Variables")
	TSoftObjectPtr<UWorld> OpenLevel;
	


	void LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess,
							const FUniqueNetId& UserID, const FString& Error);
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleted(bool bWasSuccessful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

};
