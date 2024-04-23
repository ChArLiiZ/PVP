// Fill out your copyright notice in the Description page of Project Settings.


#include "PVPGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"


UPVPGameInstance::UPVPGameInstance()
{
	
}

void UPVPGameInstance::Init()
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			//Bind Delegates
			//SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPVPGameInstance::OnCreateSessionComplete);
			//SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPVPGameInstance::OnFindSessionsComplete);
			//SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPVPGameInstance::OnJoinSessionComplete);

		}
	}
	
}

void UPVPGameInstance::Shutdown()
{
	Super::Shutdown();
	DestroySession();
}

void UPVPGameInstance::OnCreateSessionComplete(FName SessionName, bool WasSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Create Session Succeeded: %d"), WasSuccess);

	if (WasSuccess)
	{
		//UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameLevel, true, "listen");
		GetWorld()->ServerTravel("/Game/MyStuff/Levels/PVP_Map?listen");
	}
}

void UPVPGameInstance::OnFindSessionsComplete(bool WasSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Find Session Succeeded: %d"), WasSuccess);

	if (WasSuccess)
	{
		BlueprintSessionResults.Empty();
		for (FOnlineSessionSearchResult SearchResult : SessionSearch->SearchResults)
		{
			FBlueprintSessionResult temp;
			temp.OnlineResult = SearchResult;
			BlueprintSessionResults.Add(temp);
			
		}
		UE_LOG(LogTemp, Warning, TEXT("Found %d Sessions"), BlueprintSessionResults.Num());
		IsSearchSessionCompleted = true;
	}
	else
	{
		IsSearchSessionCompleted = false;
	}
}

void UPVPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	
}

void UPVPGameInstance::CreateSession(bool IsLan, int32 PlayerNum)
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Session"));

	
	
	//Session Settings
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = IsLan;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = PlayerNum;
		
	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UPVPGameInstance::FindSessions(bool IsLan, int32 MaxResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Finding Session"));
	IsSearchSessionCompleted = false;
	//Search Settings
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = IsLan;
	SessionSearch->MaxSearchResults = MaxResult;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UPVPGameInstance::JoinSession()
{
	
}

void UPVPGameInstance::DestroySession()
{
	if (!MySessionName.IsNone())
	{
		SessionInterface->DestroySession(MySessionName);
	}
	
}


