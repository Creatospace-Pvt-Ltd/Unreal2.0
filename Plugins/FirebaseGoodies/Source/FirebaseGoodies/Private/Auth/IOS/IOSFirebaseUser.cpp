#include "IOSFirebaseUser.h"

#include "Async/Async.h"
#include "IOSFirebaseUserInfo.h"

#include "Auth/IOS/IOSFirebaseAuthCredentials.h"
class IOSFirebaseAuthCredentials;

IOSFirebaseUser::IOSFirebaseUser(FIRUser* User) {
    [User retain];
    NativeUser = User;
}

IOSFirebaseUser::~IOSFirebaseUser() {
    if (NativeUser) {
        [NativeUser release];
    }

    NativeUser = nil;
}

FString IOSFirebaseUser::GetUid() {
    return FString(NativeUser.uid);
}

FString IOSFirebaseUser::GetProviderId() {
    return FString(NativeUser.uid);
}

FString IOSFirebaseUser::GetDisplayName() {
    return FString(NativeUser.displayName);
}

FString IOSFirebaseUser::GetPhotoUrl() {
    return FString(NativeUser.photoURL.absoluteString);
}

FString IOSFirebaseUser::GetEmail() {
    return FString(NativeUser.email);
}

FString IOSFirebaseUser::GetPhoneNumber() {
    return FString(NativeUser.phoneNumber);
}

bool IOSFirebaseUser::IsUserInfoValid() {
    return NativeUser != nil;
}

bool IOSFirebaseUser::IsAnonymous() {
    return NativeUser.isAnonymous;
}

bool IOSFirebaseUser::IsUserValid() {
    return NativeUser != nil;
}

void IOSFirebaseUser::Delete(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    [NativeUser deleteWithCompletion:^(NSError* _Nullable error) {
      if (error != nil) {
          FString errorString = FString(error.localizedDescription);
          AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
          return;
      }

      UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(GetUid());
    }];
}

void IOSFirebaseUser::LinkWithCredentials(UFGAuthCredentials* Credentials, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError) {
    const auto IosCredentials = StaticCastSharedPtr<IOSFirebaseAuthCredentials>(Credentials->GetCredentials())->GetNativeCredentials();
    [NativeUser linkWithCredential:IosCredentials
                        completion:^(FIRAuthDataResult* _Nullable result, NSError* _Nullable error) {
                          if (error != nil) {
                              FString errorString = FString(error.localizedDescription);
                              AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
                              return;
                          }

                          auto IosUser = MakeShareable(new IOSFirebaseUser(result.user));

                          AsyncTask(ENamedThreads::GameThread, [=]() {
                              UFGFirebaseUser* user = NewObject<UFGFirebaseUser>();
                              user->Init(IosUser);
                              UFGFirebaseUser::OnUserUpdatedCallback.ExecuteIfBound(user);
                          });
                        }];
}

void IOSFirebaseUser::Reauthenticate(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    const auto IosCredentials = StaticCastSharedPtr<IOSFirebaseAuthCredentials>(Credentials->GetCredentials())->GetNativeCredentials();
    [NativeUser reauthenticateWithCredential:IosCredentials
                                  completion:^(FIRAuthDataResult* _Nullable result, NSError* _Nullable error) {
                                    if (error != nil) {
                                        FString errorString = FString(error.localizedDescription);
                                        AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
                                        return;
                                    }

                                    AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(GetUid()); });
                                  }];
}

void IOSFirebaseUser::Reload(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    [NativeUser reloadWithCompletion:^(NSError* _Nullable error) {
      if (error != nil) {
          FString errorString = FString(error.localizedDescription);
          AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
          return;
      }

      AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(GetUid()); });
    }];
}

void IOSFirebaseUser::SendEmailVerification(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    [NativeUser sendEmailVerificationWithCompletion:^(NSError* _Nullable error) {
      if (error != nil) {
          FString errorString = FString(error.localizedDescription);
          AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
          return;
      }

      AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(GetUid()); });
    }];
}

void IOSFirebaseUser::UnlinkProvider(const FString& Provider, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError) {
    [NativeUser unlinkFromProvider:Provider.GetNSString()
                        completion:^(FIRUser* _Nullable result, NSError* _Nullable error) {
                          if (error != nil) {
                              AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), FString(error.localizedDescription)); });
                              return;
                          }

                          const auto IosUser = MakeShareable(new IOSFirebaseUser(result));

                          AsyncTask(ENamedThreads::GameThread, [=]() {
                              UFGFirebaseUser* user = NewObject<UFGFirebaseUser>();
                              user->Init(IosUser);

                              UFGFirebaseUser::OnUserUpdatedCallback.ExecuteIfBound(user);
                          });
                        }];
}

void IOSFirebaseUser::UpdateEmail(const FString& Email, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    [NativeUser updateEmail:Email.GetNSString()
                 completion:^(NSError* _Nullable error) {
                   if (error != nil) {
                       FString errorString = FString(error.localizedDescription);
                       AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
                       return;
                   }

                   AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(GetUid()); });
                 }];
}

void IOSFirebaseUser::UpdatePassword(const FString& Password, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    [NativeUser updatePassword:Password.GetNSString()
                    completion:^(NSError* _Nullable error) {
                      if (error != nil) {
                          FString errorString = FString(error.localizedDescription);
                          AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
                          return;
                      }

                      AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(GetUid()); });
                    }];
}

void IOSFirebaseUser::UpdatePhoneNumber(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    const auto IosCredentials = StaticCastSharedPtr<IOSFirebaseAuthCredentials>(Credentials->GetCredentials())->GetNativeCredentials();
    [NativeUser updatePhoneNumberCredential:(FIRPhoneAuthCredential*)IosCredentials
                                 completion:^(NSError* _Nullable error) {
                                   if (error != nil) {
                                       FString errorString = FString(error.localizedDescription);
                                       AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
                                       return;
                                   }

                                   AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(GetUid()); });
                                 }];
}

void IOSFirebaseUser::UpdateProfile(const FString& DisplayName, const FString& AvatarUrl, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    FIRUserProfileChangeRequest* request = NativeUser.profileChangeRequest;
    request.displayName = DisplayName.GetNSString();
    request.photoURL = [NSURL URLWithString:AvatarUrl.GetNSString()];
    [request commitChangesWithCompletion:^(NSError* _Nullable error) {
      if (error != nil) {
          FString errorString = FString(error.localizedDescription);
          AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
          return;
      }

      AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserVoidTaskCompletedCallback.ExecuteIfBound(GetUid()); });
    }];
}

void IOSFirebaseUser::GetToken(bool ForceRefresh, const FOnUserStringTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) {
    [NativeUser getIDTokenForcingRefresh:ForceRefresh
                              completion:^(NSString* token, NSError* error) {
                                if (error != nil) {
                                    FString errorString = FString(error.localizedDescription);
                                    AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserOperationErrorCallback.ExecuteIfBound(GetUid(), errorString); });
                                    return;
                                }

                                FString TokenString = FString(token);
                                AsyncTask(ENamedThreads::GameThread, [=]() { UFGFirebaseUser::OnUserStringTaskCompletedCallback.ExecuteIfBound(GetUid(), TokenString); });
                              }];
}

TArray<TSharedPtr<IFirebaseUserInfo>> IOSFirebaseUser::GetProviderData() {
    TArray<TSharedPtr<IFirebaseUserInfo>> Result;
    for (id<FIRUserInfo> info in NativeUser.providerData) {
        Result.Add(MakeShareable(new IOSFirebaseUserInfo(info)));
    }
    return Result;
}

bool IOSFirebaseUser::IsEmailVerified() {
    return NativeUser.emailVerified;
}
