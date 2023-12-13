// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "FeatureType.h"

#include "BillingJNIUtils.generated.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif




UCLASS()
class UBillingJNIUtils : public UObject
{
    GENERATED_BODY()

public:

#if PLATFORM_ANDROID
    static struct FProductDetails BuildProductDetails(JNIEnv* env, jobject jProductDetails);
    static struct FPurchase BuildPurchase(JNIEnv* env, jobject jPurchase);
    static struct FPurchaseHistoryRecord BuildPurchaseHistory(JNIEnv* env, jobject jPurchaseHistory);
#endif
    static FString ConvertFeatureType(const EFeatureType FeatureType);
};