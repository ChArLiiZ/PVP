// Fill out your copyright notice in the Description page of Project Settings.


#include "PVPGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

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
