// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "FeatureType.generated.h"

UENUM(BlueprintType)
enum EFeatureType
{
    SUBSCRIPTIONS               = 0     UMETA(DisplayName = "Subscriptions"),
    SUBSCRIPTIONS_UPDATE        = 1     UMETA(DisplayName = "Subscriptions Update"),
    PRICE_CHANGE_CONFIRMATION   = 2     UMETA(DisplayName = "Price Change Confirmation"),
    IN_APP_MESSAGING            = 3     UMETA(DisplayName = "In App Messaging"),
    PRODUCT_DETAILS             = 4     UMETA(DisplayName = "Product Details")
};
