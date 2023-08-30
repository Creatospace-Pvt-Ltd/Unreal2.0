// Copyright 2022 John Parmenter, All Rights Reserved.

#include "ClipboardNodesBPLibrary.h"
#include "Runtime/ApplicationCore/Public/HAL/PlatformApplicationMisc.h"
#include "ClipboardNodes.h"

UClipboardNodesBPLibrary::UClipboardNodesBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// EMPTY
}

void UClipboardNodesBPLibrary::CopyToClipboard(const FString& ToText)
{
	FPlatformApplicationMisc::ClipboardCopy(*ToText);
}

void UClipboardNodesBPLibrary::PasteFromClipboard(FString &FromText)
{
	FPlatformApplicationMisc::ClipboardPaste(FromText);
}