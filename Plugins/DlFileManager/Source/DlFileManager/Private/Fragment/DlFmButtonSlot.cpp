// Copyright(c) 2022 Dragon Li Software Limited. All Rights Reserved.


#include "Fragment/DlFmButtonSlot.h"

#include "Components/Widget.h"
#include "Fragment/SDlFmButton.h"

/////////////////////////////////////////////////////
// UDlFmButtonSlot

UDlFmButtonSlot::UDlFmButtonSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Padding = FMargin(4, 2);

	HorizontalAlignment = HAlign_Center;
	VerticalAlignment = VAlign_Center;
}

void UDlFmButtonSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Button.Reset();
}

void UDlFmButtonSlot::BuildSlot(TSharedRef<SDlFmButton> InButton)
{
	Button = InButton;

	Button.Pin()->SetContentPadding(Padding);
	Button.Pin()->SetHAlign(HorizontalAlignment);
	Button.Pin()->SetVAlign(VerticalAlignment);

	Button.Pin()->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

void UDlFmButtonSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetContentPadding(InPadding);
	}
}

void UDlFmButtonSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetHAlign(InHorizontalAlignment);
	}
}

void UDlFmButtonSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetVAlign(InVerticalAlignment);
	}
}

void UDlFmButtonSlot::SynchronizeProperties()
{
	SetPadding(Padding);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}
