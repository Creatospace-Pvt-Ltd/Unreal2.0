// Copyright Porret Gaming 2023, All Rights Reserved.

#include "../Public/BillingJNIUtils.h"

#include "../Public/ProductDetails.h"
#include "../Public/Purchase.h"
#include "../Public/PurchaseHistoryRecord.h"
#include "../Public/FeatureType.h"
#include "../Public/ProductType.h"
#include "../Public/RecurrenceMode.h"
#include "../Public/PurchaseState.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif

#if PLATFORM_ANDROID

FProductDetails UBillingJNIUtils::BuildProductDetails(JNIEnv* env, jobject jProductDetails)
{
    /*
        Retrieve the class and method IDs
    */
    // Product Details
    jclass productDetailsClass = env->GetObjectClass(jProductDetails);
    jmethodID getOneTimePurchaseOfferDetailsMethodID = env->GetMethodID(productDetailsClass, "getOneTimePurchaseOfferDetails", "()Lcom/android/billingclient/api/ProductDetails$OneTimePurchaseOfferDetails;");
    jmethodID getDescriptionMethodID = env->GetMethodID(productDetailsClass, "getDescription", "()Ljava/lang/String;");
    jmethodID getNameMethodID = env->GetMethodID(productDetailsClass, "getName", "()Ljava/lang/String;");
    jmethodID getProductIdMethodID = env->GetMethodID(productDetailsClass, "getProductId", "()Ljava/lang/String;");
    jmethodID getProductTypeMethodID = env->GetMethodID(productDetailsClass, "getProductType", "()Ljava/lang/String;");
    jmethodID getTitleMethodID = env->GetMethodID(productDetailsClass, "getTitle", "()Ljava/lang/String;");
    jmethodID getSubscriptionOfferDetailsMethodID = env->GetMethodID(productDetailsClass, "getSubscriptionOfferDetails", "()Ljava/util/List;");
    // One Time Purchase Offer Details
    jclass oneTimePurchaseOfferDetailsClass = env->FindClass("com/android/billingclient/api/ProductDetails$OneTimePurchaseOfferDetails");
    jmethodID getPriceAmountMicrosOneTimeMethodID = env->GetMethodID(oneTimePurchaseOfferDetailsClass, "getPriceAmountMicros", "()J");
    jmethodID getFormattedPriceOneTimeMethodID = env->GetMethodID(oneTimePurchaseOfferDetailsClass, "getFormattedPrice", "()Ljava/lang/String;");
    jmethodID getPriceCurrencyCodeOneTimeMethodID = env->GetMethodID(oneTimePurchaseOfferDetailsClass, "getPriceCurrencyCode", "()Ljava/lang/String;");
    // Subscription Offer Details
    jclass subscriptionOfferDetailsClass = env->FindClass("com/android/billingclient/api/ProductDetails$SubscriptionOfferDetails");
    jmethodID getPricingPhasesMethodID = env->GetMethodID(subscriptionOfferDetailsClass, "getPricingPhases", "()Lcom/android/billingclient/api/ProductDetails$PricingPhases;");
    jmethodID getBasePlanIdMethodID = env->GetMethodID(subscriptionOfferDetailsClass, "getBasePlanId", "()Ljava/lang/String;");
    jmethodID getOfferIdMethodID = env->GetMethodID(subscriptionOfferDetailsClass, "getOfferId", "()Ljava/lang/String;");
    jmethodID getOfferTokenMethodID = env->GetMethodID(subscriptionOfferDetailsClass, "getOfferToken", "()Ljava/lang/String;");
    jmethodID getOfferTagsMethodID = env->GetMethodID(subscriptionOfferDetailsClass, "getOfferTags", "()Ljava/util/List;");
    // Pricing Phases
    jclass pricingPhasesClass = env->FindClass("com/android/billingclient/api/ProductDetails$PricingPhases");
    jmethodID getPricingPhaseListMethodID = env->GetMethodID(pricingPhasesClass, "getPricingPhaseList", "()Ljava/util/List;");
    // PricingPhase
    jclass pricingPhaseClass = env->FindClass("com/android/billingclient/api/ProductDetails$PricingPhase");
    jmethodID getBillingCycleCountMethodID = env->GetMethodID(pricingPhaseClass, "getBillingCycleCount", "()I");
    jmethodID getRecurrenceModeMethodID = env->GetMethodID(pricingPhaseClass, "getRecurrenceMode", "()I");
    jmethodID getPriceAmountMicrosPricingPhaseMethodID = env->GetMethodID(pricingPhaseClass, "getPriceAmountMicros", "()J");
    jmethodID getBillingPeriodMethodID = env->GetMethodID(pricingPhaseClass, "getBillingPeriod", "()Ljava/lang/String;");
    jmethodID getFormattedPricePricingPhaseMethodID = env->GetMethodID(pricingPhaseClass, "getFormattedPrice", "()Ljava/lang/String;");
    jmethodID getPriceCurrencyCodePricingPhaseMethodID = env->GetMethodID(pricingPhaseClass, "getPriceCurrencyCode", "()Ljava/lang/String;");

    /*
        Call Methods to extract JNI fields
    */
    // Product Details
    jstring descriptionString = (jstring)env->CallObjectMethod(jProductDetails, getDescriptionMethodID);
    jstring nameString = (jstring)env->CallObjectMethod(jProductDetails, getNameMethodID);
    jstring productIdString = (jstring)env->CallObjectMethod(jProductDetails, getProductIdMethodID);
    jstring productTypeString = (jstring)env->CallObjectMethod(jProductDetails, getProductTypeMethodID);
    jstring titleString = (jstring)env->CallObjectMethod(jProductDetails, getTitleMethodID);
    // One Time Purchase Offer Details
    jobject jOneTimePurchaseOfferDetails = env->CallObjectMethod(jProductDetails, getOneTimePurchaseOfferDetailsMethodID);
    jlong priceAmountMicrosOneTimeLong = 0;
    jstring formattedPriceString = nullptr;
    jstring priceCurrencyCodeString = nullptr;
    if (jOneTimePurchaseOfferDetails != nullptr)
    {
        priceAmountMicrosOneTimeLong = env->CallLongMethod(jOneTimePurchaseOfferDetails, getPriceAmountMicrosOneTimeMethodID);
        formattedPriceString = (jstring)env->CallObjectMethod(jOneTimePurchaseOfferDetails, getFormattedPriceOneTimeMethodID);
        priceCurrencyCodeString = (jstring)env->CallObjectMethod(jOneTimePurchaseOfferDetails, getPriceCurrencyCodeOneTimeMethodID);
    }
    else
    {
        formattedPriceString = env->NewStringUTF("");
        priceCurrencyCodeString = env->NewStringUTF("");
    }
    
    // Subscription Offer Details
    //      Since this is a list, We will extract it straight into c++ within the loops instead of creating many temporary variables
    jobject jSubscriptionOfferDetailsList = env->CallObjectMethod(jProductDetails, getSubscriptionOfferDetailsMethodID);
    TArray<FSubscriptionOfferDetails> SubscriptionDetails;
    if (jSubscriptionOfferDetailsList != nullptr)
    {
        jsize jSubListSize = env->CallIntMethod(jSubscriptionOfferDetailsList, env->GetMethodID(env->GetObjectClass(jSubscriptionOfferDetailsList), "size", "()I"));
        SubscriptionDetails.SetNum(static_cast<int32>(jSubListSize));
        for (jsize i = 0; i < jSubListSize; ++i) {
            // Call Methods to extract JNI fields
            jobject jSubscriptionOfferDetails = env->CallObjectMethod(jSubscriptionOfferDetailsList, env->GetMethodID(env->GetObjectClass(jSubscriptionOfferDetailsList), "get", "(I)Ljava/lang/Object;"), i);
            
            jobject jPricingPhases = env->CallObjectMethod(jSubscriptionOfferDetails, getPricingPhasesMethodID);
            jobject jPricingPhasesList = env->CallObjectMethod(jPricingPhases, getPricingPhaseListMethodID);
            jsize jPricingPhasesListSize = env->CallIntMethod(jPricingPhasesList, env->GetMethodID(env->GetObjectClass(jPricingPhasesList), "size", "()I"));
            TArray<FPricingPhase> PricingPhases;
            PricingPhases.SetNum(static_cast<int32>(jPricingPhasesListSize));
            for (jsize j = 0; j < jPricingPhasesListSize; ++j)
            {
                // Call Methods to extract JNI fields
                jobject jPricingPhase = env->CallObjectMethod(jPricingPhasesList, env->GetMethodID(env->GetObjectClass(jPricingPhasesList), "get", "(I)Ljava/lang/Object;"), j);

                jint billingCycleCountInt = env->CallIntMethod(jPricingPhase, getBillingCycleCountMethodID);
                jint recurrenceModeInt = env->CallIntMethod(jPricingPhase, getRecurrenceModeMethodID);
                jlong priceAmountMicrosPricingPhaseLong = env->CallLongMethod(jPricingPhase, getPriceAmountMicrosPricingPhaseMethodID);
                jstring billingPeriodString = (jstring)env->CallObjectMethod(jPricingPhase, getBillingPeriodMethodID);
                jstring formattedPricePricingPhaseString = (jstring)env->CallObjectMethod(jPricingPhase, getFormattedPricePricingPhaseMethodID);
                jstring priceCurrencyCodePricingPhaseString = (jstring)env->CallObjectMethod(jPricingPhase, getPriceCurrencyCodePricingPhaseMethodID);

                // Convert to c++
                PricingPhases[static_cast<int32>(j)] = FPricingPhase(
                    static_cast<int32>(billingCycleCountInt),
                    static_cast<ERecurrenceMode>(recurrenceModeInt - 1),
                    static_cast<int64>(priceAmountMicrosPricingPhaseLong),
                    FString(UTF8_TO_TCHAR(env->GetStringUTFChars(billingPeriodString, NULL))),
                    FString(UTF8_TO_TCHAR(env->GetStringUTFChars(formattedPricePricingPhaseString, NULL))),
                    FString(UTF8_TO_TCHAR(env->GetStringUTFChars(priceCurrencyCodePricingPhaseString, NULL))));

                // cleanup
                env->DeleteLocalRef(billingPeriodString);
                env->DeleteLocalRef(formattedPricePricingPhaseString);
                env->DeleteLocalRef(priceCurrencyCodePricingPhaseString);
                env->DeleteLocalRef(jPricingPhase);
            }

            jstring basePlanIdString = (jstring)env->CallObjectMethod(jSubscriptionOfferDetails, getBasePlanIdMethodID);
            jstring offerIdString = (jstring)env->CallObjectMethod(jSubscriptionOfferDetails, getOfferIdMethodID);
            jstring offerTokenString = (jstring)env->CallObjectMethod(jSubscriptionOfferDetails, getOfferTokenMethodID);

            jobject jOfferTagsList = env->CallObjectMethod(jSubscriptionOfferDetails, getOfferTagsMethodID);
            jsize jTagListSize = env->CallIntMethod(jOfferTagsList, env->GetMethodID(env->GetObjectClass(jOfferTagsList), "size", "()I"));
            TArray<FString> Tags;
            Tags.SetNum(static_cast<int32>(jTagListSize));
            for (jsize j = 0; j < jTagListSize; ++j)
            {
                // Call Methods to extract JNI fields
                jstring jTagString = (jstring) env->CallObjectMethod(jOfferTagsList, env->GetMethodID(env->GetObjectClass(jOfferTagsList), "get", "(I)Ljava/lang/Object;"), j);

                // Convert to c++
                Tags[static_cast<int32>(j)] = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(jTagString, NULL)));

                // cleanup
                env->DeleteLocalRef(jTagString);
            }
            // Convert to c++
            SubscriptionDetails[static_cast<int32>(i)] = FSubscriptionOfferDetails(
                PricingPhases,
                FString(UTF8_TO_TCHAR(env->GetStringUTFChars(basePlanIdString, NULL))),
                FString(UTF8_TO_TCHAR(env->GetStringUTFChars(offerIdString, NULL))),
                FString(UTF8_TO_TCHAR(env->GetStringUTFChars(offerTokenString, NULL))),
                Tags);
            // cleanup
            env->DeleteLocalRef(jSubscriptionOfferDetails);
            env->DeleteLocalRef(jPricingPhases);
            env->DeleteLocalRef(jPricingPhasesList);
            env->DeleteLocalRef(basePlanIdString);
            env->DeleteLocalRef(offerIdString);
            env->DeleteLocalRef(offerTokenString);
            env->DeleteLocalRef(jOfferTagsList);
        }
    }
    else
    {
         SubscriptionDetails = TArray<FSubscriptionOfferDetails>();
    }

    /*
        Convert JNI types to C++ types
    */
    // Product Details
    FString ProductDescription = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(descriptionString, NULL)));
    FString ProductName = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(nameString, NULL)));
    FString ProductID = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(productIdString, NULL)));
    EProductType ProductType = (FString(UTF8_TO_TCHAR(env->GetStringUTFChars(productTypeString, NULL))) == TEXT("inapp")) ? EProductType::INAPP : EProductType::SUBS;
    FString ProductTitle = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(titleString, NULL)));
    // One Time Purchase Offer Details
    int64 PriceAmountMicrosOneTime = static_cast<int64>(priceAmountMicrosOneTimeLong);
    FString FormattedPrice = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(formattedPriceString, NULL)));
    FString PriceCurrencyCode = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(priceCurrencyCodeString, NULL)));

    /*
        Cleanup
    */ 
    // Product Details
    env->ReleaseStringUTFChars(descriptionString, env->GetStringUTFChars(descriptionString, NULL));
    env->ReleaseStringUTFChars(nameString, env->GetStringUTFChars(nameString, NULL));
    env->ReleaseStringUTFChars(productIdString, env->GetStringUTFChars(productIdString, NULL));
    env->ReleaseStringUTFChars(productTypeString, env->GetStringUTFChars(productTypeString, NULL));
    env->ReleaseStringUTFChars(titleString, env->GetStringUTFChars(titleString, NULL));
    // One Time Purchase Offer Details
    env->ReleaseStringUTFChars(formattedPriceString, env->GetStringUTFChars(formattedPriceString, NULL));
    env->ReleaseStringUTFChars(priceCurrencyCodeString, env->GetStringUTFChars(priceCurrencyCodeString, NULL));
    env->DeleteLocalRef(jOneTimePurchaseOfferDetails);
    // Subscription Offer Details
    env->DeleteLocalRef(jSubscriptionOfferDetailsList);
  
    return FProductDetails(FOneTimePurchaseOfferDetails(PriceAmountMicrosOneTime, FormattedPrice, PriceCurrencyCode), ProductDescription, ProductName, ProductID, ProductType, ProductTitle, SubscriptionDetails);
}

FPurchaseHistoryRecord UBillingJNIUtils::BuildPurchaseHistory(JNIEnv* env, jobject jPurchaseHistory)
{
    /*
        Retrieve the class and method IDs
    */
    // Purchase History Record
    jclass purchaseHistoryClass = env->GetObjectClass(jPurchaseHistory);
    jmethodID getQuantityMethodID = env->GetMethodID(purchaseHistoryClass, "getQuantity", "()I");
    jmethodID getPurchaseTimeMethodID = env->GetMethodID(purchaseHistoryClass, "getPurchaseTime", "()J");
    jmethodID getDeveloperPayloadMethodID = env->GetMethodID(purchaseHistoryClass, "getDeveloperPayload", "()Ljava/lang/String;");
    jmethodID getPurchaseTokenMethodID = env->GetMethodID(purchaseHistoryClass, "getPurchaseToken", "()Ljava/lang/String;");
    jmethodID getSignatureMethodID = env->GetMethodID(purchaseHistoryClass, "getSignature", "()Ljava/lang/String;");
    jmethodID getProductsMethodID = env->GetMethodID(purchaseHistoryClass, "getProducts", "()Ljava/util/List;");

    /*
        Call Methods to extract JNI fields
    */
    // Purchase History Record
    jint quantityInt = env->CallIntMethod(jPurchaseHistory, getQuantityMethodID);
    jlong purchaseTimeLong = env->CallLongMethod(jPurchaseHistory, getPurchaseTimeMethodID);
    jstring developerPayloadString = (jstring)env->CallObjectMethod(jPurchaseHistory, getDeveloperPayloadMethodID);
    jstring purchaseTokenString = (jstring)env->CallObjectMethod(jPurchaseHistory, getPurchaseTokenMethodID);
    jstring signatureString = (jstring)env->CallObjectMethod(jPurchaseHistory, getSignatureMethodID);
    // Handle Products
    jobject jProductsList = env->CallObjectMethod(jPurchaseHistory, getProductsMethodID);
    jsize jProductListSize = env->CallIntMethod(jProductsList, env->GetMethodID(env->GetObjectClass(jProductsList), "size", "()I"));
    TArray<FString> Products;
    Products.SetNum(static_cast<int32>(jProductListSize));
    for (jsize i = 0; i < jProductListSize; ++i)
    {
        // Call Methods to extract JNI fields
        jstring jProductString = (jstring) env->CallObjectMethod(jProductsList, env->GetMethodID(env->GetObjectClass(jProductsList), "get", "(I)Ljava/lang/Object;"), i);
        // Convert to c++
        Products[static_cast<int32>(i)] = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(jProductString, NULL)));
        // cleanup
        env->DeleteLocalRef(jProductString);
    }

    /*
        Convert JNI types to C++ types
    */
    // Purchase History Record
    int32 Quantity = static_cast<int32>(quantityInt);
    int64 PurchaseTime = static_cast<int64>(purchaseTimeLong);
    FString DeveloperPayload = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(developerPayloadString, NULL)));
    FString PurchaseToken = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(purchaseTokenString, NULL)));
    FString Signature = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(signatureString, NULL)));
    // Tags are already done

    /*
        Cleanup
    */
    // Purchase History Record
    env->ReleaseStringUTFChars(developerPayloadString, env->GetStringUTFChars(developerPayloadString, NULL));
    env->ReleaseStringUTFChars(purchaseTokenString, env->GetStringUTFChars(purchaseTokenString, NULL));
    env->ReleaseStringUTFChars(signatureString, env->GetStringUTFChars(signatureString, NULL));
    env->DeleteLocalRef(jProductsList);

    return FPurchaseHistoryRecord(Quantity, PurchaseTime, DeveloperPayload, PurchaseToken, Signature, Products);
}

FPurchase UBillingJNIUtils::BuildPurchase(JNIEnv* env, jobject jPurchase)
{
    /*
        Retrieve the class and method IDs
    */
    // Purchase
    jclass purchaseClass = env->GetObjectClass(jPurchase);
    jmethodID getPurchaseStateMethodID = env->GetMethodID(purchaseClass, "getPurchaseState", "()I");
    jmethodID getQuantityMethodID = env->GetMethodID(purchaseClass, "getQuantity", "()I");
    jmethodID getPurchaseTimeMethodID = env->GetMethodID(purchaseClass, "getPurchaseTime", "()J");
    jmethodID getAccountIdentifiersMethodID = env->GetMethodID(purchaseClass, "getAccountIdentifiers", "()Lcom/android/billingclient/api/AccountIdentifiers;");
    jmethodID getDeveloperPayloadMethodID = env->GetMethodID(purchaseClass, "getDeveloperPayload", "()Ljava/lang/String;");
    jmethodID getOrderIdMethodID = env->GetMethodID(purchaseClass, "getOrderId", "()Ljava/lang/String;");
    jmethodID getPackageNameMethodID = env->GetMethodID(purchaseClass, "getPackageName", "()Ljava/lang/String;");
    jmethodID getPurchaseTokenMethodID = env->GetMethodID(purchaseClass, "getPurchaseToken", "()Ljava/lang/String;");
    jmethodID getSignatureMethodID = env->GetMethodID(purchaseClass, "getSignature", "()Ljava/lang/String;");
    jmethodID getProductsMethodID = env->GetMethodID(purchaseClass, "getProducts", "()Ljava/util/List;");
    jmethodID isAcknowledgedMethodID = env->GetMethodID(purchaseClass, "isAcknowledged", "()Z");
    jmethodID isAutoRenewingMethodID = env->GetMethodID(purchaseClass, "isAutoRenewing", "()Z");
    // AccountIdentifiers
    jclass accountIdentifiersClass = env->FindClass("com/android/billingclient/api/AccountIdentifiers");
    jmethodID getObfuscatedAccountIdMethodID = env->GetMethodID(accountIdentifiersClass, "getObfuscatedAccountId", "()Ljava/lang/String;");
    jmethodID getObfuscatedProfileIdMethodID = env->GetMethodID(accountIdentifiersClass, "getObfuscatedProfileId", "()Ljava/lang/String;");
    
    /*
        Call Methods to extract JNI fields
    */
    // Purchase
    jint purchaseStateInt = env->CallIntMethod(jPurchase, getPurchaseStateMethodID);
    jint quantityInt = env->CallIntMethod(jPurchase, getQuantityMethodID);
    jlong purchaseTimeLong = env->CallLongMethod(jPurchase, getPurchaseTimeMethodID);
    jstring developerPayloadString = (jstring)env->CallObjectMethod(jPurchase, getDeveloperPayloadMethodID);
    jstring orderIdString = (jstring)env->CallObjectMethod(jPurchase, getOrderIdMethodID);
    jstring packageNameString = (jstring)env->CallObjectMethod(jPurchase, getPackageNameMethodID);
    jstring purchaseTokenString = (jstring)env->CallObjectMethod(jPurchase, getPurchaseTokenMethodID);
    jstring signatureString = (jstring)env->CallObjectMethod(jPurchase, getSignatureMethodID);
    jboolean isAcknowledgedBoolean = env->CallBooleanMethod(jPurchase, isAcknowledgedMethodID);
    jboolean isAutoRenewingBoolean = env->CallBooleanMethod(jPurchase, isAutoRenewingMethodID);
    // Handle Purchase.Tags
    jobject jProductsList = env->CallObjectMethod(jPurchase, getProductsMethodID);
    jsize jProductListSize = env->CallIntMethod(jProductsList, env->GetMethodID(env->GetObjectClass(jProductsList), "size", "()I"));
    TArray<FString> Products;
    Products.SetNum(static_cast<int32>(jProductListSize));
    for (jsize i = 0; i < jProductListSize; ++i)
    {
        // Call Methods to extract JNI fields
        jstring jProductString = (jstring) env->CallObjectMethod(jProductsList, env->GetMethodID(env->GetObjectClass(jProductsList), "get", "(I)Ljava/lang/Object;"), i);
        // Convert to c++
        Products[static_cast<int32>(i)] = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(jProductString, NULL)));
        // cleanup
        env->DeleteLocalRef(jProductString);
    }

    // AccountIdentifiers
    jobject jAccountIdentifiers = env->CallObjectMethod(jPurchase, getAccountIdentifiersMethodID);
    jstring obfuscatedAccountIdString = nullptr;
    jstring obfuscatedProfileIdString = nullptr;
    if (jAccountIdentifiers != nullptr)
    {
        obfuscatedAccountIdString = (jstring)env->CallObjectMethod(jAccountIdentifiers, getObfuscatedAccountIdMethodID);
        obfuscatedProfileIdString = (jstring)env->CallObjectMethod(jAccountIdentifiers, getObfuscatedProfileIdMethodID);
    }
    else
    {
        obfuscatedAccountIdString = env->NewStringUTF("");
        obfuscatedProfileIdString = env->NewStringUTF("");
    }

    /*
        Convert JNI types to C++ types
    */
    // Purchase
    EPurchaseState PurchaseState = static_cast<EPurchaseState>(purchaseStateInt);
    int32 Quantity = static_cast<int32>(quantityInt);
    int64 PurchaseTime = static_cast<int64>(purchaseTimeLong);
    FString DeveloperPayload = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(developerPayloadString, NULL)));
    FString OrderId = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(orderIdString, NULL)));
    FString PackageName = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(packageNameString, NULL)));
    FString PurchaseToken = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(purchaseTokenString, NULL)));
    FString Signature = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(signatureString, NULL)));
    bool IsAcknowledged = (isAcknowledgedBoolean == JNI_TRUE);
    bool IsAutoRenewing = (isAutoRenewingBoolean == JNI_TRUE);
    // Tags are already done
    // AccountIdentifiers
    FString ObfuscatedAccountId = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(obfuscatedAccountIdString, NULL)));
    FString ObfuscatedProfileId = FString(UTF8_TO_TCHAR(env->GetStringUTFChars(obfuscatedProfileIdString, NULL)));

    /*
        Cleanup
    */
    // Purchase
    env->ReleaseStringUTFChars(developerPayloadString, env->GetStringUTFChars(developerPayloadString, NULL));
    env->ReleaseStringUTFChars(orderIdString, env->GetStringUTFChars(orderIdString, NULL));
    env->ReleaseStringUTFChars(packageNameString, env->GetStringUTFChars(packageNameString, NULL));
    env->ReleaseStringUTFChars(purchaseTokenString, env->GetStringUTFChars(purchaseTokenString, NULL));
    env->ReleaseStringUTFChars(signatureString, env->GetStringUTFChars(signatureString, NULL));
    env->DeleteLocalRef(jProductsList);
    // AccountIdentifiers
    env->ReleaseStringUTFChars(obfuscatedAccountIdString, env->GetStringUTFChars(obfuscatedAccountIdString, NULL));
    env->ReleaseStringUTFChars(obfuscatedProfileIdString, env->GetStringUTFChars(obfuscatedProfileIdString, NULL));
    env->DeleteLocalRef(jAccountIdentifiers);

    return FPurchase(PurchaseState, Quantity, PurchaseTime, FAccountIdentifiers(ObfuscatedAccountId, ObfuscatedProfileId), DeveloperPayload, OrderId, PackageName, PurchaseToken, Signature, Products, IsAcknowledged, IsAutoRenewing);
}

#endif

FString UBillingJNIUtils::ConvertFeatureType(const EFeatureType FeatureType)
{
    if (FeatureType == EFeatureType::SUBSCRIPTIONS)
    {
        return TEXT("subscriptions");
    }

    if (FeatureType == EFeatureType::SUBSCRIPTIONS_UPDATE)
    {
        return TEXT("subscriptionsUpdate");
    }

    if (FeatureType == EFeatureType::PRICE_CHANGE_CONFIRMATION)
    {
        return TEXT("priceChangeConfirmation");
    }

    if (FeatureType == EFeatureType::IN_APP_MESSAGING)
    {
        return TEXT("bbb");
    }

    if (FeatureType == EFeatureType::PRODUCT_DETAILS)
    {
        return TEXT("fff");
    }

    return TEXT("");
}