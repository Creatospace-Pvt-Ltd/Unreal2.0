// Copyright(c) 2022 Dragon Li Software Limited. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Fragment/DlFmButton.h"
#include "Components/CheckBox.h"
#include "DlFmFilteredToggleButton.generated.h"

/**
 * 
 */
UCLASS()
class DLFILEMANAGER_API UDlFmFilteredToggleButton : public UDlFmButton
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category="CheckBox|Event")
	FOnCheckBoxComponentStateChanged OnCheckStateChanged;
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetIsChecked(bool InIsChecked);
	//Called by DlCombo, will not trigger Event, subscribe combo instead to get info
	void SetIsChecked_Internal(bool InIsChecked);
	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool IsChecked() const;

	virtual void SynchronizeProperties() override;


private:
	//UPROPERTY(EditAnywhere, Category = "Filtered")
	//float GapTime = 0.3;
	UPROPERTY(EditAnywhere, Category = "Toggle")
	FSlateBrush whenTrue;
	UPROPERTY(EditAnywhere, Category = "Toggle")
	FSlateBrush whenHoveredTrue;
	UPROPERTY(EditAnywhere, Category = "Toggle")
	FSlateBrush whenDisabledTrue;
	UPROPERTY(EditAnywhere, Category = "Toggle")
	FSlateBrush whenFalse;
	UPROPERTY(EditAnywhere, Category = "Toggle")
	FSlateBrush whenHoveredFalse;
	UPROPERTY(EditAnywhere, Category = "Toggle")
	FSlateBrush whenDisabledFalse;
	UPROPERTY(EditAnywhere, Category = "Toggle")
	bool initBool = true;
	UFUNCTION()
	void DlFilteredClick_UpperHandle();

	bool currentBool;
	//float lastChangeTime = 0;
	FButtonStyle cacheButtonStyle;
	void _update_Style();
};
