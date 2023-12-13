// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "BillingResponseCode.generated.h"

UENUM(BlueprintType)
enum EBillingResponseCode
{
    FEATURE_NOT_SUPPORTED = -2      UMETA(DisplayName = "FEATURE_NOT_SUPPORTED"),
    SERVICE_DISCONNECTED = -1       UMETA(DisplayName = "SERVICE_DISCONNECTED"),
    OK = 0                          UMETA(DisplayName = "OK"),
    USER_CANCELED = 1               UMETA(DisplayName = "USER_CANCELED"),
    SERVICE_UNAVAILABLE = 2         UMETA(DisplayName = "SERVICE_UNAVAILABLE"),
    BILLING_UNAVAILABLE = 3         UMETA(DisplayName = "BILLING_UNAVAILABLE"),
    ITEM_UNAVAILABLE = 4            UMETA(DisplayName = "ITEM_UNAVAILABLE"),
    DEVELOPER_ERROR = 5             UMETA(DisplayName = "DEVELOPER_ERROR"),
    ERROR = 6                       UMETA(DisplayName = "ERROR"),
    ITEM_ALREADY_OWNED = 7          UMETA(DisplayName = "ITEM_ALREADY_OWNED"),
    ITEM_NOT_OWNED = 8              UMETA(DisplayName = "ITEM_NOT_OWNED"),
    NETWORK_ERROR = 1               UMETA(DisplayName = "NETWORK_ERROR")
};
