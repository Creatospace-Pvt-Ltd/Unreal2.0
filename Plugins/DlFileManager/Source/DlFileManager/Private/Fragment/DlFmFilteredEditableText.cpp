// Copyright(c) 2022 Dragon Li Software Limited. All Rights Reserved.


#include "Fragment/DlFmFilteredEditableText.h"

UDlFmFilteredEditableText::UDlFmFilteredEditableText(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OnTextCommitted.AddUniqueDynamic(this,&UDlFmFilteredEditableText::OnFilteredTextCommitted_InternalHandle);
}

void UDlFmFilteredEditableText::OnFilteredTextCommitted_InternalHandle(const FText& TextVal, ETextCommit::Type CommitMethodVal)
{
	if (GetOwningPlayer()->GetWorld()->RealTimeSeconds >= LastTextCommittedTime+ GapTime)
	{
		LastTextCommittedTime = GetOwningPlayer()->GetWorld()->RealTimeSeconds;
		OnFilteredTextCommitted.Broadcast(TextVal,CommitMethodVal);
	}
}
