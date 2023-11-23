// Fill out your copyright notice in the Description page of Project Settings.


#include "Calendar.h"
#include<iostream>
#include<ctime>
#include<iomanip>

FString UCalendar::text(FString Name)
{
	return FString(Name);
}

void UCalendar::GetMonthDetails(int32 Year, int32 Month, int32& NumberOfDaysInTheMonth, int32& FirstDay,FString& Month_Name)
{
    if (Month < 1 || Month > 12)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid month. Month should be between 1 and 12."));
        return;
    }

    // Calculating the number of days in the month
    if (Month == 2)
    {
        // Checking for Leap Year
        if ((Year % 4 == 0 && Year % 100 != 0) || (Year % 400 == 0))
        {
            NumberOfDaysInTheMonth = 29;
        }
        else
        {
            NumberOfDaysInTheMonth = 28;
        }
    }
    else if (Month == 4 || Month == 6 || Month == 9 || Month == 11)
    {
        NumberOfDaysInTheMonth = 30;
    }
    else
    {
        NumberOfDaysInTheMonth = 31;
    }

    // Calculate the first day of the month
    struct tm TimeInfo = { 0 };
    TimeInfo.tm_year = Year - 1900;
    TimeInfo.tm_mon = Month - 1;
    TimeInfo.tm_mday = 1;

    std::mktime(&TimeInfo);

    FirstDay = TimeInfo.tm_wday;

    FString ar[12] = { "Janurary","Feburary","March","April","May","June",
                        "July","August","September","October","November","December" };

    Month_Name = ar[Month-1];
}

