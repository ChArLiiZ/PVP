// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
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
	UPVPGameInstance();

	UPROPERTY()
	FName MySessionName = "PVP";

	UPROPERTY(BlueprintReadOnly)
	bool IsSearchSessionCompleted = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<FBlueprintSessionResult> BlueprintSessionResults;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> GameLevel;

	UFUNCTION(BlueprintCallable)
	void CreateSession(bool IsLan, int32 PlayerNum);

	UFUNCTION(BlueprintCallable)
	void FindSessions(bool IsLan, int32 MaxResult);

	UFUNCTION(BlueprintCallable)
	void JoinSession();

	UFUNCTION(BlueprintCallable)
	void DestroySession();


	
protected:
	
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	virtual void Init() override;
	virtual void Shutdown() override;

	//Delegates
	virtual void OnCreateSessionComplete(FName SessionName, bool WasSuccess);
	virtual void OnFindSessionsComplete(bool WasSuccess);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

};
