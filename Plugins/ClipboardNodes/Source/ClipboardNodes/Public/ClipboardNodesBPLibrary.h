// Copyright 2022 John Parmenter, All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ClipboardNodesBPLibrary.generated.h"

UCLASS()
class UClipboardNodesBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	/**
	 * Copies the inputted FString to the OS clipboard.
	 * @param	ToText	FString to copy to clipboard
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Copy To Clipboard", Keywords = "Copy Paste Clipboard"), Category = "Clipboard")
	static void CopyToClipboard(const FString& ToText);

	/**
	 * Pastes the current text from the OS clipboard to a FString.
	 * @param	FromText	FString from current clipboard text
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Paste From Clipboard", Keywords = "Copy Paste Clipboard"), Category = "Clipboard")
	static void PasteFromClipboard(FString &FromText);
};
