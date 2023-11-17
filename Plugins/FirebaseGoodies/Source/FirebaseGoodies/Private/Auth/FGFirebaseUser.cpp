// Copyright (c) 2022 Nineva Studios

#include "Auth/FGFirebaseUser.h"

#include "Auth/FGAuthLibrary.h"
#include "Auth/FGFirebaseUserInfo.h"
#include "FirebaseGoodiesLog.h"

#include "Interface/IFirebaseUser.h"

FOnUserVoidTaskCompleted UFGFirebaseUser::OnUserVoidTaskCompletedCallback;
FOnUserUpdated UFGFirebaseUser::OnUserUpdatedCallback;
FOnUserOperationError UFGFirebaseUser::OnUserOperationErrorCallback;
FOnUserStringTaskCompleted UFGFirebaseUser::OnUserStringTaskCompletedCallback;

UFGFirebaseUser::~UFGFirebaseUser()
{
}

bool UFGFirebaseUser::IsAnonymous()
{
	if (!IsUserValid())
	{
		return false;
	}

	return UserImpl->IsAnonymous();
}


bool UFGFirebaseUser::IsEmailVerified() const
{
	if (!IsUserValid())
	{
		return false;
	}

	return UserImpl->IsEmailVerified();
}

bool UFGFirebaseUser::IsUserValid() const
{
	const bool IsValid = UserImpl->IsUserValid();
	if (!IsValid)
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("User is not valid."));
	}
	return IsValid;
}

void UFGFirebaseUser::Delete(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserVoidTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid())
	{
		return;
	}

	UserImpl->Delete(OnSuccess, OnError);
}

void UFGFirebaseUser::LinkWithCredentials(UFGAuthCredentials* credentials, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserUpdatedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid() || !credentials->AreValid())
	{
		return;
	}

	UserImpl->LinkWithCredentials(credentials, OnSuccess, OnError);
}

void UFGFirebaseUser::Reauthenticate(UFGAuthCredentials* credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserVoidTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid() || !credentials->AreValid())
	{
		return;
	}

	UserImpl->Reauthenticate(credentials, OnSuccess, OnError);
}

void UFGFirebaseUser::Reload(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserVoidTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid())
	{
		return;
	}

	UserImpl->Reload(OnSuccess, OnError);
}

void UFGFirebaseUser::SendEmailVerification(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserVoidTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid())
	{
		return;
	}

	UserImpl->SendEmailVerification(OnSuccess, OnError);
}

void UFGFirebaseUser::UnlinkProvider(const FString& Provider, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserUpdatedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid())
	{
		return;
	}

	UserImpl->UnlinkProvider(Provider, OnSuccess, OnError);
}

void UFGFirebaseUser::UpdateEmail(const FString& Email, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserVoidTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid())
	{
		return;
	}

	UserImpl->UpdateEmail(Email, OnSuccess, OnError);
}

void UFGFirebaseUser::UpdatePassword(const FString& Password, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserVoidTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid())
	{
		return;
	}

	UserImpl->UpdatePassword(Password, OnSuccess, OnError);
}

void UFGFirebaseUser::UpdatePhoneNumber(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess,
                                        const FOnUserOperationError& OnError)
{
	OnUserVoidTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid() || !Credentials->AreValid())
	{
		return;
	}

	UserImpl->UpdatePhoneNumber(Credentials, OnSuccess, OnError);
}

void UFGFirebaseUser::UpdateProfile(const FString& displayName, const FString& avatarUrl, const FOnUserVoidTaskCompleted& OnSuccess,
                                    const FOnUserOperationError& OnError)
{
	OnUserVoidTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid())
	{
		return;
	}

	UserImpl->UpdateProfile(displayName, avatarUrl, OnSuccess, OnError);
}

void UFGFirebaseUser::GetToken(bool ForceRefresh, const FOnUserStringTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	OnUserStringTaskCompletedCallback = OnSuccess;
	OnUserOperationErrorCallback = OnError;

	if (!IsUserValid())
	{
		return;
	}

	// TODO update to also get claims
	UserImpl->GetToken(ForceRefresh, OnSuccess, OnError);
}

TArray<UFGFirebaseUserInfo*> UFGFirebaseUser::GetProviderData()
{
	TArray<UFGFirebaseUserInfo*> Result;

	for (const auto& Provider : UserImpl->GetProviderData())
	{
		UFGFirebaseUserInfo* Info = NewObject<UFGFirebaseUserInfo>();
		Info->Init(Provider);
		Result.Add(Info);
	}

	return Result;
}

void UFGFirebaseUser::Init(const TSharedPtr<IFirebaseUser> User)
{
	UserImpl = User;
	UserInfoImpl = User;
}
