// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "PurchaseState.generated.h"

UENUM(BlueprintType)
enum EPurchaseState
{
    UNSPECIFIED_STATE   = 0     UMETA(DisplayName = "Unspecified"),
    PURCHASED           = 1     UMETA(DisplayName = "Purchased"),
    PENDING             = 2     UMETA(DisplayName = "Pending")
};
