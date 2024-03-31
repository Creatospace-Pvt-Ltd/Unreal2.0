// Copyright(c) 2022 Dragon Li Software Limited. All Rights Reserved.


#include "Fragment/DlFmFilteredToggleButton.h"

UDlFmFilteredToggleButton::UDlFmFilteredToggleButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OnFilteredClicked.AddUniqueDynamic(this, &UDlFmFilteredToggleButton::DlFilteredClick_UpperHandle);

	currentBool = initBool;
	cacheButtonStyle = WidgetStyle;
	_update_Style();
}

void UDlFmFilteredToggleButton::DlFilteredClick_UpperHandle()
{
	
	currentBool = !currentBool;
	_update_Style();
	//lastChangeTime = GetOwningPlayer()->GetWorld()->RealTimeSeconds;
	OnCheckStateChanged.Broadcast(currentBool);
	
}

void UDlFmFilteredToggleButton::SetIsChecked(bool InIsChecked)
{
	if(InIsChecked!=currentBool)
	{
		currentBool = InIsChecked;
		_update_Style();
		OnCheckStateChanged.Broadcast(currentBool);		
	}
}

void UDlFmFilteredToggleButton::SetIsChecked_Internal(bool InIsChecked)
{
	if(InIsChecked!=currentBool)
	{
		currentBool = InIsChecked;
		_update_Style();	
	}
}

bool UDlFmFilteredToggleButton::IsChecked() const
{
	return currentBool;
}

void UDlFmFilteredToggleButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	_update_Style();
}

void UDlFmFilteredToggleButton::_update_Style()
{
	if (currentBool==true)
	{
		cacheButtonStyle.SetNormal(whenTrue);
		cacheButtonStyle.SetHovered(whenHoveredTrue);
		cacheButtonStyle.SetPressed(whenFalse);
		cacheButtonStyle.SetDisabled(whenDisabledTrue);
	}
	else
	{
		cacheButtonStyle.SetNormal(whenFalse);
		cacheButtonStyle.SetHovered(whenHoveredFalse);
		cacheButtonStyle.SetPressed(whenTrue);
		cacheButtonStyle.SetDisabled(whenDisabledFalse);
	}
	SetStyle(cacheButtonStyle);
}
