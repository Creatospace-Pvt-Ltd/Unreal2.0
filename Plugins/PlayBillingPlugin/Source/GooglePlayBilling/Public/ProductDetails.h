// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "ProductType.h"
#include "RecurrenceMode.h"
#include "ProductDetails.generated.h"

USTRUCT(BlueprintType)
struct FOneTimePurchaseOfferDetails
{
    GENERATED_BODY()

    FOneTimePurchaseOfferDetails() : FOneTimePurchaseOfferDetails(0, TEXT(""), TEXT(""))
	{}

	FOneTimePurchaseOfferDetails(int64 _PriceAmountMicros, FString _FormattedPrice, FString _PriceCurrencyCode)
	{
        PriceAmountMicros = _PriceAmountMicros;
        FormattedPrice = _FormattedPrice;
        PriceCurrencyCode = _PriceCurrencyCode;
	}

    UPROPERTY(BlueprintReadOnly, Category = "OneTimePurchaseOfferDetails")
    int64 PriceAmountMicros;

    UPROPERTY(BlueprintReadOnly, Category = "OneTimePurchaseOfferDetails")
    FString FormattedPrice;

    UPROPERTY(BlueprintReadOnly, Category = "OneTimePurchaseOfferDetails")
    FString PriceCurrencyCode;

};

USTRUCT(BlueprintType)
struct FPricingPhase
{
    GENERATED_BODY()

    FPricingPhase() : FPricingPhase(0, ERecurrenceMode::FINITE_RECURRING, 0, TEXT(""), TEXT(""), TEXT(""))
	{}

	FPricingPhase(int32 _BillingCycleCount, TEnumAsByte<ERecurrenceMode> _RecurrenceMode, int64 _PriceAmountMicros, FString _BillingPeriod, FString _FormattedPrice, FString _PriceCurrencyCode)
	{
        BillingCycleCount = _BillingCycleCount;
        RecurrenceMode = _RecurrenceMode;
        PriceAmountMicros = _PriceAmountMicros;
        BillingPeriod = _BillingPeriod;
        FormattedPrice = _FormattedPrice;
        PriceCurrencyCode = _PriceCurrencyCode;
	}

    UPROPERTY(BlueprintReadOnly, Category = "PricingPhase")
    int32 BillingCycleCount;

    UPROPERTY(BlueprintReadOnly, Category = "PricingPhase")
    TEnumAsByte<ERecurrenceMode> RecurrenceMode;

    UPROPERTY(BlueprintReadOnly, Category = "PricingPhase")
    int64 PriceAmountMicros;

    UPROPERTY(BlueprintReadOnly, Category = "PricingPhase")
    FString BillingPeriod;

    UPROPERTY(BlueprintReadOnly, Category = "PricingPhase")
    FString FormattedPrice;

    UPROPERTY(BlueprintReadOnly, Category = "PricingPhase")
    FString PriceCurrencyCode;

};

USTRUCT(BlueprintType)
struct FSubscriptionOfferDetails
{
    GENERATED_BODY()

    FSubscriptionOfferDetails() : FSubscriptionOfferDetails({}, TEXT(""), TEXT(""), TEXT(""), {})
	{}

	FSubscriptionOfferDetails(TArray<FPricingPhase> _PricingPhases, FString _BasePlanId, FString _OfferId, FString _OfferToken, TArray<FString> _OfferTags)
	{
        PricingPhases = _PricingPhases;
        BasePlanId = _BasePlanId;
        OfferId = _OfferId;
        OfferToken = _OfferToken;
        OfferTags = _OfferTags;
	}

    UPROPERTY(BlueprintReadOnly, Category = "SubscriptionOfferDetails")
    TArray<FPricingPhase> PricingPhases;

    UPROPERTY(BlueprintReadOnly, Category = "SubscriptionOfferDetails")
    FString BasePlanId;

    UPROPERTY(BlueprintReadOnly, Category = "SubscriptionOfferDetails")
    FString OfferId;

    UPROPERTY(BlueprintReadOnly, Category = "SubscriptionOfferDetails")
    FString OfferToken;

    UPROPERTY(BlueprintReadOnly, Category = "SubscriptionOfferDetails")
    TArray<FString> OfferTags;

};

USTRUCT(BlueprintType)
struct FProductDetails
{
    GENERATED_BODY()

    FProductDetails() : FProductDetails(FOneTimePurchaseOfferDetails(), TEXT(""), TEXT(""), TEXT(""), EProductType::INAPP, TEXT(""), {})
	{}

	FProductDetails(FOneTimePurchaseOfferDetails _OneTimePurchaseOfferDetails, FString _Description, FString _Name, FString _ProductId, TEnumAsByte<EProductType> _ProductType, FString _Title, TArray<FSubscriptionOfferDetails> _SubscriptionOfferDetails)
	{
        OneTimePurchaseOfferDetails = _OneTimePurchaseOfferDetails;
        Description = _Description;
        Name = _Name;
        ProductId = _ProductId;
        ProductType = _ProductType;
        Title = _Title;
        SubscriptionOfferDetails = _SubscriptionOfferDetails;
	}

    UPROPERTY(BlueprintReadOnly, Category = "ProductDetails")
    FOneTimePurchaseOfferDetails OneTimePurchaseOfferDetails;

    UPROPERTY(BlueprintReadOnly, Category = "ProductDetails")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "ProductDetails")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "ProductDetails")
    FString ProductId;

    UPROPERTY(BlueprintReadOnly, Category = "ProductDetails")
    TEnumAsByte<EProductType> ProductType;

    UPROPERTY(BlueprintReadOnly, Category = "ProductDetails")
    FString Title;

    UPROPERTY(BlueprintReadOnly, Category = "ProductDetails")
    TArray<FSubscriptionOfferDetails> SubscriptionOfferDetails;

};
