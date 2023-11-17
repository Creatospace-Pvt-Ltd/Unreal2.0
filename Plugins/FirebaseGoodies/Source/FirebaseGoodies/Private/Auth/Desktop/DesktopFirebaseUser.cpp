#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT

#include "DesktopFirebaseUser.h"

#include "Auth/Desktop/DesktopFirebaseUserInfo.h"
#include "FirebaseGoodiesLog.h"
#include "DesktopFirebaseAuthCredentials.h"
#include "Async/Async.h"

DesktopFirebaseUser::DesktopFirebaseUser(firebase::auth::User* User)
{
	DesktopUser = User;
}

bool DesktopFirebaseUser::IsAnonymous()
{
	return DesktopUser->is_anonymous();
}

bool DesktopFirebaseUser::IsUserValid()
{
	return DesktopUser != nullptr;
}

void DesktopFirebaseUser::Delete(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto Task = DesktopUser->Delete();
	Task.AddOnCompletion([=](const firebase::Future<void>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str());
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to delete user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::LinkWithCredentials(UFGAuthCredentials* Credentials, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto FirebaseAuthCredentials = StaticCastSharedPtr<DesktopFirebaseAuthCredentials>(Credentials->GetCredentials());
	const auto Task = DesktopUser->LinkWithCredential(FirebaseAuthCredentials->GetDesktopCredential());
	Task.AddOnCompletion([=](const firebase::Future<firebase::auth::User*>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			const auto NewDesktopUser = MakeShareable(new DesktopFirebaseUser(*Callback.result()));

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser* User = NewObject<UFGFirebaseUser>();
				User->Init(NewDesktopUser);
				UFGFirebaseUser::OnUserUpdatedCallback.ExecuteIfBound(User);
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Link With Credentials error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::Reauthenticate(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto FirebaseAuthCredentials = StaticCastSharedPtr<DesktopFirebaseAuthCredentials>(Credentials->GetCredentials());
	const auto Task = DesktopUser->Reauthenticate(FirebaseAuthCredentials->GetDesktopCredential());
	Task.AddOnCompletion([=](const firebase::Future<void>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str());
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to reauthenticate user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::Reload(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto Task = DesktopUser->Reload();
	Task.AddOnCompletion([=](const firebase::Future<void>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str());
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to reload user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::SendEmailVerification(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto Task = DesktopUser->SendEmailVerification();
	Task.AddOnCompletion([=](const firebase::Future<void>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str());
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to Send Email Verification for user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::UnlinkProvider(const FString& Provider, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto Task = DesktopUser->Unlink(TCHAR_TO_ANSI(*Provider));
	Task.AddOnCompletion([=](const firebase::Future<firebase::auth::User*>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			const auto NewDesktopUser = MakeShareable(new DesktopFirebaseUser(*Callback.result()));

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser* User = NewObject<UFGFirebaseUser>();
				User->Init(NewDesktopUser);
				UFGFirebaseUser::OnUserUpdatedCallback.ExecuteIfBound(User);
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Link With Credentials error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::UpdateEmail(const FString& Email, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto Task = DesktopUser->UpdateEmail(TCHAR_TO_ANSI(*Email));
	Task.AddOnCompletion([=](const firebase::Future<void>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str());
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to Send Email Verification for user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::UpdatePassword(const FString& Password, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto Task = DesktopUser->UpdatePassword(TCHAR_TO_ANSI(*Password));
	Task.AddOnCompletion([=](const firebase::Future<void>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str());
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to Send Email Verification for user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::UpdatePhoneNumber(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto FirebaseAuthCredentials = StaticCastSharedPtr<DesktopFirebaseAuthCredentials>(Credentials->GetCredentials());
	const auto Task = DesktopUser->UpdatePhoneNumberCredential(FirebaseAuthCredentials->GetDesktopCredential());
	Task.AddOnCompletion([=](const firebase::Future<firebase::auth::User*>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str());
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to Send Email Verification for user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::UpdateProfile(const FString& DisplayName, const FString& AvatarUrl, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	firebase::auth::User::UserProfile Profile;
	Profile.display_name = TCHAR_TO_ANSI(*DisplayName);
	Profile.photo_url = TCHAR_TO_ANSI(*AvatarUrl);

	const auto Task = DesktopUser->UpdateUserProfile(Profile);
	Task.AddOnCompletion([=](const firebase::Future<void>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str());
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to Send Email Verification for user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

void DesktopFirebaseUser::GetToken(bool ForceRefresh, const FOnUserStringTaskCompleted& OnSuccess, const FOnUserOperationError& OnError)
{
	const auto Task = DesktopUser->GetToken(ForceRefresh);
	Task.AddOnCompletion([=](const firebase::Future<std::string>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			const FString Token = (*Callback.result()).c_str();
			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserStringTaskCompletedCallback.ExecuteIfBound(DesktopUser->uid().c_str(), Token);
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to Send Email Verification for user: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]
			{
				UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(DesktopUser->uid().c_str(), ErrorMessage);
			});
		}
	});
}

TArray<TSharedPtr<IFirebaseUserInfo>> DesktopFirebaseUser::GetProviderData()
{
	TArray<TSharedPtr<IFirebaseUserInfo>> Result;
	for (firebase::auth::User::UserInfoInterface* UserInfo : DesktopUser->provider_data())
	{
		Result.Add(MakeShareable(new DesktopFirebaseUserInfo(UserInfo)));
	}
	return Result;
}

FString DesktopFirebaseUser::GetUid()
{
	return DesktopUser->uid().c_str();
}

FString DesktopFirebaseUser::GetProviderId()
{
	return DesktopUser->provider_id().c_str();
}

FString DesktopFirebaseUser::GetDisplayName()
{
	return DesktopUser->display_name().c_str();
}

FString DesktopFirebaseUser::GetPhotoUrl()
{
	return DesktopUser->photo_url().c_str();
}

FString DesktopFirebaseUser::GetEmail()
{
	return DesktopUser->email().c_str();
}

FString DesktopFirebaseUser::GetPhoneNumber()
{
	return DesktopUser->phone_number().c_str();
}

bool DesktopFirebaseUser::IsEmailVerified()
{
	return DesktopUser->is_email_verified();
}

bool DesktopFirebaseUser::IsUserInfoValid()
{
	return DesktopUser != nullptr;
}

#endif
