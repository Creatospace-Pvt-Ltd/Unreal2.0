// Copyright Porret Gaming 2023, All Rights Reserved.

#pragma once

#include "RecurrenceMode.generated.h"

UENUM(BlueprintType)
enum ERecurrenceMode
{
    INFINITE_RECURRING  = 0     UMETA(DisplayName = "Infinite Recurring"),
    FINITE_RECURRING    = 1     UMETA(DisplayName = "Finite Recurring"),
    NON_RECURRING       = 2     UMETA(DisplayName = "Non Recurring")
};
