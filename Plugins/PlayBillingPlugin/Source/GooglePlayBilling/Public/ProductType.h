// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "ProductType.generated.h"

UENUM(BlueprintType)
enum EProductType
{
    INAPP   = 0     UMETA(DisplayName = "In App Products"),
    SUBS    = 1     UMETA(DisplayName = "Subscriptions")
};
