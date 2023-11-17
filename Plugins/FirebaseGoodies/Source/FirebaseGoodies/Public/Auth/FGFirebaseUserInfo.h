// Copyright (c) 2022 Nineva Studios

#pragma once

class IFirebaseUserInfo;

#include "FGFirebaseUserInfo.generated.h"

UCLASS(BlueprintType)
class FIREBASEGOODIES_API UFGFirebaseUserInfo : public UObject
{
	GENERATED_BODY()

public:
	virtual ~UFGFirebaseUserInfo() override;

	/**
	 * @return a string used to uniquely identify your user in your Firebase project's user database.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Firebase User")
	FString GetUid() const;

	/**
	 * @return the provider ID for the user (For example, "Facebook").
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Firebase User")
	FString GetProviderId() const;

	/**
	 * @return the display name associated with the user, if any.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Firebase User")
	FString GetDisplayName() const;

	/**
	 * @return the photo url associated with the user, if any.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Firebase User")
	FString GetPhotoUrl() const;

	/**
	 * @return the email associated with the user, if any.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Firebase User")
	FString GetEmail() const;

	/**
	 * @return the phone number associated with the user, if any.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Firebase User")
	FString GetPhoneNumber() const;

	/**
	 * Convenience method to check if the native iOS/Android user info object is valid.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Firebase User")
	bool IsUserInfoValid() const;


	void Init(TSharedPtr<IFirebaseUserInfo> UserInfo);

protected:
	TSharedPtr<IFirebaseUserInfo> UserInfoImpl;
};
