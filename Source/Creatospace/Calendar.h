// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Calendar.generated.h"

/**
 * 
 */
UCLASS()
class CREATOSPACE_API UCalendar : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "GetCalander")
		static FString text(FString Name);
		
public:
	UFUNCTION(BlueprintCallable, Category = "DateUtils")
	static void GetMonthDetails(int32 Year, int32 Month_idx, int32& NumberOfDaysInTheMonth, int32& FirstDay, FString& Month_Name);
	
};
