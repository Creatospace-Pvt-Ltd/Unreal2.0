#pragma once

#include "AndroidDeepLinkFunctions.generated.h"

UCLASS(NotBlueprintable)
class UAndroidDeepLinkFunctions : public UObject {
	GENERATED_BODY()
	
public:

#if PLATFORM_ANDROID
	static void InitJavaFunctions();
#endif

	/**
	 * Gets Source URL in deep links
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AndroidAPI")
		FString deepLinkURL = "";

	UFUNCTION(BlueprintCallable, meta = (Keywords = "Deep Link ", DisplayName = "Get Android Deep Link"), Category = "AndroidAPI")
		FString AndroidGetDeepLink();
};