// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "PurchaseState.h"
#include "Purchase.generated.h"

USTRUCT(BlueprintType)
struct FAccountIdentifiers
{
    GENERATED_BODY()

    FAccountIdentifiers(): FAccountIdentifiers(TEXT(""), TEXT(""))
    {}

    FAccountIdentifiers(FString _ObfuscatedAccountId, FString _ObfuscatedProfileId)
    {
        ObfuscatedAccountId = _ObfuscatedAccountId;
        ObfuscatedProfileId = _ObfuscatedProfileId;
    }

    UPROPERTY(BlueprintReadOnly, Category = "AccountIdentifiers")
    FString ObfuscatedAccountId;

    UPROPERTY(BlueprintReadOnly, Category = "AccountIdentifiers")
    FString ObfuscatedProfileId;

};

USTRUCT(BlueprintType)
struct FPurchase
{
    GENERATED_BODY()

    FPurchase() : FPurchase(EPurchaseState::UNSPECIFIED_STATE, 0, 0, FAccountIdentifiers(), TEXT(""), TEXT(""), TEXT(""), TEXT(""), TEXT(""), {}, false, false)
    {}

	FPurchase(
        TEnumAsByte<EPurchaseState> _PurchaseState,
        int32 _Quantity,
        int64 _PurchaseTime,
        FAccountIdentifiers _AccountIdentifiers,
        FString _DeveloperPayload,
        FString _OrderId,
        FString _PackageName,
        FString _PurchaseToken,
        FString _Signature,
        TArray<FString> _Products,
        bool _isAcknowledged,
        bool _isAutoRenewing)
	{
        PurchaseState = _PurchaseState;
        Quantity = _Quantity;
        PurchaseTime = _PurchaseTime;
        AccountIdentifiers = _AccountIdentifiers;
        DeveloperPayload = _DeveloperPayload;
        OrderId = _OrderId;
        PackageName = _PackageName;
        PurchaseToken = _PurchaseToken;
        Signature = _Signature;
        Products = _Products;
        isAcknowledged = _isAcknowledged;
        isAutoRenewing = _isAutoRenewing;
	}

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    TEnumAsByte<EPurchaseState> PurchaseState;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    int32 Quantity;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    int64 PurchaseTime;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    FAccountIdentifiers AccountIdentifiers;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    FString DeveloperPayload;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    FString OrderId;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    FString PackageName;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    FString PurchaseToken;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    FString Signature;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    TArray<FString> Products;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    bool isAcknowledged;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase")
    bool isAutoRenewing;

};