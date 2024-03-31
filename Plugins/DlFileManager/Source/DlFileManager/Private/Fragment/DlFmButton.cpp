// Copyright(c) 2022 Dragon Li Software Limited. All Rights Reserved.

#include "Fragment/DlFmButton.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Fragment/SDlFmButton.h"
#include "Fragment/DlFmButtonSlot.h"
#include "Styling/UMGCoreStyle.h"
#include "Blueprint/WidgetTree.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UDlFmButton

static FButtonStyle* DefaultButtonStyle = nullptr;

#if WITH_EDITOR
static FButtonStyle* EditorButtonStyle = nullptr;
#endif 

UDlFmButton::UDlFmButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (DefaultButtonStyle == nullptr)
	{
		DefaultButtonStyle = new FButtonStyle(FUMGCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"));

		// Unlink UMG default colors.
		DefaultButtonStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultButtonStyle;

#if WITH_EDITOR 
	if (EditorButtonStyle == nullptr)
	{
		EditorButtonStyle = new FButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("EditorUtilityButton"));

		// Unlink UMG Editor colors from the editor settings colors.
		EditorButtonStyle->UnlinkColors();
	}

	if (IsEditorWidget())
	{
		WidgetStyle = *EditorButtonStyle;

		// The CDO isn't an editor widget and thus won't use the editor style, call post edit change to mark difference from CDO
		PostEditChange();
	}
#endif // WITH_EDITOR

	ColorAndOpacity = FLinearColor::White;
	BackgroundColor = FLinearColor::White;

	ClickMethod = EButtonClickMethod::DownAndUp;
	TouchMethod = EButtonTouchMethod::DownAndUp;

	IsFocusable = true;

#if WITH_EDITORONLY_DATA
	AccessibleBehavior = ESlateAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

void UDlFmButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyButton.Reset();
}

TSharedRef<SWidget> UDlFmButton::RebuildWidget()
{
	MyButton = SNew(SDlFmButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject( this, &ThisClass::SlateHandleHovered )
		.OnUnhovered_UObject( this, &ThisClass::SlateHandleUnhovered )
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.Paint_OffsetPercent(Paint_OffsetPercent)//Dl+
		.Paint_OverflowPercent(Paint_OverflowPercent)//Dl+
		.IsFocusable(IsFocusable)
		;

	if ( GetChildrenCount() > 0 )
	{
		Cast<UDlFmButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}
	
	return MyButton.ToSharedRef();
}

void UDlFmButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyButton->SetColorAndOpacity( ColorAndOpacity );
	MyButton->SetBorderBackgroundColor( BackgroundColor );
}

UClass* UDlFmButton::GetSlotClass() const
{
	return UDlFmButtonSlot::StaticClass();
}

void UDlFmButton::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if ( MyButton.IsValid() )
	{
		CastChecked<UDlFmButtonSlot>(InSlot)->BuildSlot(MyButton.ToSharedRef());
	}
}

void UDlFmButton::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyButton.IsValid() )
	{
		MyButton->SetContent(SNullWidget::NullWidget);
	}
}

void UDlFmButton::SetStyle(const FButtonStyle& InStyle)
{
	WidgetStyle = InStyle;
	if ( MyButton.IsValid() )
	{
		MyButton->SetButtonStyle(&WidgetStyle);
	}
}

void UDlFmButton::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if ( MyButton.IsValid() )
	{
		MyButton->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UDlFmButton::SetBackgroundColor(FLinearColor InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
	if ( MyButton.IsValid() )
	{
		MyButton->SetBorderBackgroundColor(InBackgroundColor);
	}
}

bool UDlFmButton::IsPressed() const
{
	if ( MyButton.IsValid() )
	{
		return MyButton->IsPressed();
	}

	return false;
}

void UDlFmButton::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
	if ( MyButton.IsValid() )
	{
		MyButton->SetClickMethod(ClickMethod);
	}
}

void UDlFmButton::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
	if ( MyButton.IsValid() )
	{
		MyButton->SetTouchMethod(TouchMethod);
	}
}

void UDlFmButton::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;
	if ( MyButton.IsValid() )
	{
		MyButton->SetPressMethod(PressMethod);
	}
}

void UDlFmButton::PostLoad()
{
	Super::PostLoad();

	if ( GetChildrenCount() > 0 )
	{
		//TODO UMG Pre-Release Upgrade, now buttons have slots of their own.  Convert existing slot to new slot.
		if ( UPanelSlot* PanelSlot = GetContentSlot() )
		{
			UDlFmButtonSlot* ButtonSlot = Cast<UDlFmButtonSlot>(PanelSlot);
			if ( ButtonSlot == NULL )
			{
				ButtonSlot = NewObject<UDlFmButtonSlot>(this);
				ButtonSlot->Content = GetContentSlot()->Content;
				ButtonSlot->Content->Slot = ButtonSlot;
				Slots[0] = ButtonSlot;
			}
		}
	}

	if( GetLinkerUEVersion() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS && Style_DEPRECATED != nullptr )
	{
		const FButtonStyle* StylePtr = Style_DEPRECATED->GetStyle<FButtonStyle>();
		if(StylePtr != nullptr)
		{
			WidgetStyle = *StylePtr;
		}

		Style_DEPRECATED = nullptr;
	}
}

FReply UDlFmButton::SlateHandleClicked()
{
	OnClicked.Broadcast();

	const float TmpTime=GetOwningPlayer()->GetWorld()->RealTimeSeconds;//Dl+
	if (TmpTime >= LastClickedTime+ GapTime)//Dl+
	{//Dl+
		LastClickedTime = TmpTime;//Dl+
		OnFilteredClicked.Broadcast();//Dl+
	}//Dl+

	return FReply::Handled();
}

void UDlFmButton::SlateHandlePressed()
{
	OnPressed.Broadcast();
	
	const float TmpTime=GetOwningPlayer()->GetWorld()->RealTimeSeconds;//Dl+
	if (TmpTime >= LastPressedTime+ GapTime)//Dl+
	{//Dl+
		LastPressedTime = TmpTime;//Dl+
		OnFilteredPressed.Broadcast();//Dl+
	}//Dl+
}

void UDlFmButton::SlateHandleReleased()
{
	OnReleased.Broadcast();
	
	const float TmpTime=GetOwningPlayer()->GetWorld()->RealTimeSeconds;//Dl+
	if (TmpTime >= LastReleasedTime+ GapTime)//Dl+
	{//Dl+
		LastReleasedTime = TmpTime;//Dl+
		OnFilteredReleased.Broadcast();//Dl+
	}//Dl+
}

void UDlFmButton::SlateHandleHovered()
{
	OnHovered.Broadcast();
}

void UDlFmButton::SlateHandleUnhovered()
{
	OnUnhovered.Broadcast();
}

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UDlFmButton::GetAccessibleWidget() const
{
	return MyButton;
}
#endif

#if WITH_EDITOR

const FText UDlFmButton::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

