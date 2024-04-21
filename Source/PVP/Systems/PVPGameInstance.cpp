// Fill out your copyright notice in the Description page of Project Settings.


#include "PVPGameInstance.h"

#include "IContentBrowserSingleton.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

void UPVPGameInstance::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = LoginType;

			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this, &UPVPGameInstance::LoginWithEOS_Return);
			IdentityPointerRef->Login(0, AccountDetails);
		}
	}
	
	
}

FString UPVPGameInstance::GetPlayerUserName()
{
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			if(IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return IdentityPointerRef->GetPlayerNickname(0);
			}
		}
	}

	return FString();
	
}

bool UPVPGameInstance::IsPlayerLoggedIn()
{
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			return IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn;
		}
	}

	return false;
	
}

void UPVPGameInstance::LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserID,
                                           const FString& Error)
{
	if (bWasSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Login Failed... Reason - %S"), *Error);
	}
}

void UPVPGameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), OpenLevel, true, "?listen");
	}
}

void UPVPGameInstance::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy Session Success"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Destroy Session Failed"));
	}
}

void UPVPGameInstance::OnFindSessionCompleted(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Session Success"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Find Session Failed"));
	}
}

void UPVPGameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FString JoinAddress;
			IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(this->GetWorld());
			if (SubSystemRef)
			{
				IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
				if (SessionPtrRef)
				{
					SessionPtrRef->GetResolvedConnectString(FName("MainSession"), JoinAddress);
					UE_LOG(LogTemp, Warning, TEXT("Join Address is %s"), *JoinAddress);
					if (!JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
					}
				}
			}
		}
	}
}

void UPVPGameInstance::CreateEOSSession(bool bIsLan, int32 NumOfPublicConnections)
{
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			//SessionSettings
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = false;
			SessionCreationInfo.bIsLANMatch = bIsLan;
			SessionCreationInfo.bAllowInvites = true;
			SessionCreationInfo.NumPublicConnections = NumOfPublicConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = true;
			SessionCreationInfo.bUsesPresence = false;
			SessionCreationInfo.bShouldAdvertise = true;
			SessionCreationInfo.Set(SEARCH_KEYWORDS, FString("PVP"), EOnlineDataAdvertisementType::ViaOnlineService);
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UPVPGameInstance::OnCreateSessionCompleted);
			SessionPtrRef->CreateSession(0, FName("MainSession"), SessionCreationInfo);
			
			
		}
	}
	
}

void UPVPGameInstance::FindSessions(int32 MaxSearchResults, TArray<FBlueprintSessionResult>& BlueprintSessionResults)
{
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch);
			SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			SessionSearch->MaxSearchResults = MaxSearchResults;
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UPVPGameInstance::OnFindSessionCompleted);
			SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
			TArray<FBlueprintSessionResult> BPResults;
			for (FOnlineSessionSearchResult SearchResult : SessionSearch->SearchResults)
			{
				FBlueprintSessionResult BlueprintSessionResult;
				BlueprintSessionResult.OnlineResult = SearchResult;
				BPResults.Add(BlueprintSessionResult);
			}
			BlueprintSessionResults = BPResults;
		}
	}
}

void UPVPGameInstance::JoinSession(int32 index)
{
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			if (SessionSearch->SearchResults[index].IsValid())
			{
				SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UPVPGameInstance::OnJoinSessionCompleted);
				SessionPtrRef->JoinSession(0, FName("MainSession"), SessionSearch->SearchResults[index]);
			}
		}
	}
	
}

void UPVPGameInstance::DestroySession()
{
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UPVPGameInstance::OnDestroySessionCompleted);
			SessionPtrRef->DestroySession(FName("MainSession"));
		}
	}
}
