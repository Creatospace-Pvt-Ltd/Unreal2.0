// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BillingResponseCode.h"
#include "FeatureType.h"
#include "ProductDetails.h"
#include "ProductType.h"
#include "Purchase.h"
#include "PurchaseHistoryRecord.h"

#include "GooglePlayBillingBPLibrary.generated.h"

/*
    Delegate Definitions
*/
DECLARE_DYNAMIC_DELEGATE(FOperationComplete_GPB);
DECLARE_DYNAMIC_DELEGATE_OneParam(FProductDetailsDelegate_GPB, const TArray<FProductDetails>&, ProductDetails);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPurchasesDelegate_GPB, const FPurchase&, Purchase);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPurchasesListDelegate_GPB, const TArray<FPurchaseHistoryRecord>&, HistoricalPurchases);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FErrorDelegate_GPB, EBillingResponseCode, BillingResponseCode, const FString&, ResponseMessage);

UCLASS()
class UGooglePlayBillingBPLibrary: public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /*
        Delegates
    */
    // Query Product Details
    static FProductDetailsDelegate_GPB QueryProductDetails_OnSuccess;
    static FErrorDelegate_GPB QueryProductDetails_OnFailure;
    // Launch Billing Flow
    static FPurchasesDelegate_GPB LaunchBillingFlow_OnSuccess;
    static FErrorDelegate_GPB LaunchBillingFlow_OnFailure;
    // Check For Purchases
    static FPurchasesDelegate_GPB CheckForPurchases_OnSuccess;
    static FErrorDelegate_GPB CheckForPurchases_OnFailure;
    // Query Purchase History
    static FPurchasesListDelegate_GPB QueryPurchaseHistory_OnSuccess;
    static FErrorDelegate_GPB QueryPurchaseHistory_OnFailure;
    // Show In App Messages
    static FOperationComplete_GPB ShowInAppMessages_OnActionTaken;
    static FErrorDelegate_GPB ShowInAppMessages_OnFailure;

    static void CallAppropriateFailureDelegate(int32 Index, int32 ResponseCode, FString ResponseMessage);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Query Product Details", Keywords = "Query Product Details"), Category = "Google Play Billing")
    static void QueryProductDetails(
		const TArray<FString>& ProductIds,
		const EProductType ProductType,
        FProductDetailsDelegate_GPB OnSuccess,
        FErrorDelegate_GPB OnFailure);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Launch Billing Flow", Keywords = "Launch Billing Flow"), Category = "Google Play Billing")
    static void LaunchBillingFlow(
		const FString& ProductId,
        const EProductType ProductType,
        const bool& IsOfferPersonalised,
        const FString& OfferToken,
        FPurchasesDelegate_GPB OnSuccess,
        FErrorDelegate_GPB OnFailure);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Check for Purchases", Keywords = "Check for Purchases"), Category = "Google Play Billing")
    static void CheckForPurchases(
        const EProductType ProductType,
        FPurchasesDelegate_GPB OnSuccess,
        FErrorDelegate_GPB OnFailure);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Query Purchase History", Keywords = "Query Purchase History"), Category = "Google Play Billing")
    static void QueryPurchaseHistory(
        const EProductType ProductType,
        FPurchasesListDelegate_GPB OnSuccess,
        FErrorDelegate_GPB OnFailure);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Show In App Messages", Keywords = "Show In App Messages"), Category = "Google Play Billing")
    static void ShowInAppMessages(
        FOperationComplete_GPB OnActionTaken,
        FErrorDelegate_GPB OnFailure);

    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Feature Supported", Keywords = "Is Feature Supported"), Category = "Google Play Billing")
    static bool IsFeatureSupported(
        const EFeatureType FeatureType);
    
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Billing Ready", Keywords = "Is Billing Ready"), Category = "Google Play Billing")
    static bool IsBillingReady();

    UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Subscription Offer Token", Keywords = "Get Subscription Offer Token"), Category = "Google Play Billing")
    static FString GetSubscriptionOfferToken(
        TArray<FProductDetails> SubscriptionProducts,
        const FString& ProductId,
        const FString& BasePlanId,
        const FString& OfferId);

};