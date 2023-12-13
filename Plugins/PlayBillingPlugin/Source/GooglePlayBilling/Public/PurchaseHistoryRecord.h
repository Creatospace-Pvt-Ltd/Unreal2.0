// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "PurchaseHistoryRecord.generated.h"

USTRUCT(BlueprintType)
struct FPurchaseHistoryRecord
{
    GENERATED_BODY()

    FPurchaseHistoryRecord() : FPurchaseHistoryRecord(0, 0, TEXT(""), TEXT(""), TEXT(""), {})
    {}

	FPurchaseHistoryRecord(
        int32 _Quantity,
        int64 _PurchaseTime,
        FString _DeveloperPayload,
        FString _PurchaseToken,
        FString _Signature,
        TArray<FString> _Products)
	{
        Quantity = _Quantity;
        PurchaseTime = _PurchaseTime;
        DeveloperPayload = _DeveloperPayload;
        PurchaseToken = _PurchaseToken;
        Signature = _Signature;
        Products = _Products; 
	}

    UPROPERTY(BlueprintReadOnly, Category = "Purchase History Record")
    int32 Quantity;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase History Record")
    int64 PurchaseTime;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase History Record")
    FString DeveloperPayload;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase History Record")
    FString PurchaseToken;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase History Record")
    FString Signature;

    UPROPERTY(BlueprintReadOnly, Category = "Purchase History Record")
    TArray<FString> Products;

};