// Copyright Porret Gaming 2023, All Rights Reserved.

#include "../Public/GooglePlayBillingBPLibrary.h"

#include "../Public/BillingJNIUtils.h"
#include "Async/TaskGraphInterfaces.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif

/*
	Query Product Details
*/

FProductDetailsDelegate_GPB UGooglePlayBillingBPLibrary::QueryProductDetails_OnSuccess;
FErrorDelegate_GPB UGooglePlayBillingBPLibrary::QueryProductDetails_OnFailure;

void UGooglePlayBillingBPLibrary::QueryProductDetails(
	const TArray<FString>& ProductIds,
	const EProductType ProductType,
	FProductDetailsDelegate_GPB OnSuccess,
	FErrorDelegate_GPB OnFailure)
{
	UGooglePlayBillingBPLibrary::QueryProductDetails_OnSuccess = OnSuccess;
	UGooglePlayBillingBPLibrary::QueryProductDetails_OnFailure = OnFailure;

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Fetch Method ID
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_QueryProductDetails", "([Ljava/lang/String;Ljava/lang/String;)V", false);
		// Convert the string array parameter
		jclass jStringClass = Env->FindClass("java/lang/String");
		jobjectArray jProductIds = Env->NewObjectArray(ProductIds.Num(), jStringClass, nullptr);
		for (int32 i = 0; i < ProductIds.Num(); i++)
		{
			jstring jProductId = Env->NewStringUTF(TCHAR_TO_UTF8(*ProductIds[i]));
			Env->SetObjectArrayElement(jProductIds, i, jProductId);
			Env->DeleteLocalRef(jProductId);
		}
		// Convert Product Type
		jstring jProductType = Env->NewStringUTF(ProductType == EProductType::INAPP ? "inapp" : "subs");
		// Call Void Method
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, jProductIds, jProductType);
		// Clean up local references
		Env->DeleteLocalRef(jStringClass);
		Env->DeleteLocalRef(jProductIds);
		Env->DeleteLocalRef(jProductType);
	}
#endif
}

/*
	Launch Billing Flow
*/
FPurchasesDelegate_GPB UGooglePlayBillingBPLibrary::LaunchBillingFlow_OnSuccess;
FErrorDelegate_GPB UGooglePlayBillingBPLibrary::LaunchBillingFlow_OnFailure;

void UGooglePlayBillingBPLibrary::LaunchBillingFlow(
	const FString& ProductId,
	const EProductType ProductType,
	const bool& IsOfferPersonalised,
	const FString& OfferToken,
	FPurchasesDelegate_GPB OnSuccess,
	FErrorDelegate_GPB OnFailure)
{
	UGooglePlayBillingBPLibrary::LaunchBillingFlow_OnSuccess = OnSuccess;
	UGooglePlayBillingBPLibrary::LaunchBillingFlow_OnFailure = OnFailure;

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Fetch Method ID
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_LaunchBillingFlow", "(Ljava/lang/String;Ljava/lang/String;ZLjava/lang/String;)V", false);
		// convert parameters to JNI types
		jstring jProductId = Env->NewStringUTF(TCHAR_TO_UTF8(*ProductId));
		jstring jOfferToken = Env->NewStringUTF(TCHAR_TO_UTF8(*OfferToken));
		jstring jProductType = Env->NewStringUTF(ProductType == EProductType::INAPP ? "inapp" : "subs");
		jboolean jIsOfferPersonalised = IsOfferPersonalised ? JNI_TRUE : JNI_FALSE;
		// Call Void Method
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, jProductId, jProductType, jIsOfferPersonalised, jOfferToken);
		// Clean up local references
		Env->DeleteLocalRef(jProductId);
		Env->DeleteLocalRef(jOfferToken);
		Env->DeleteLocalRef(jProductType);
	}
#endif
}

/*
	Check for Purchases made while app was not running
*/
FPurchasesDelegate_GPB UGooglePlayBillingBPLibrary::CheckForPurchases_OnSuccess;
FErrorDelegate_GPB UGooglePlayBillingBPLibrary::CheckForPurchases_OnFailure;

void UGooglePlayBillingBPLibrary::CheckForPurchases(
	const EProductType ProductType,
	FPurchasesDelegate_GPB OnSuccess,
	FErrorDelegate_GPB OnFailure)
{
	UGooglePlayBillingBPLibrary::CheckForPurchases_OnSuccess = OnSuccess;
	UGooglePlayBillingBPLibrary::CheckForPurchases_OnFailure = OnFailure;

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Fetch Method ID
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CheckForPurchases", "(Ljava/lang/String;)V", false);
		// convert parameters to JNI types
		jstring jProductType = Env->NewStringUTF(ProductType == EProductType::INAPP ? "inapp" : "subs");
		// Call Void Method
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, jProductType);
		// Clean up local references
		Env->DeleteLocalRef(jProductType);
	}
#endif
}

/*
	Query Purchase History
*/
FPurchasesListDelegate_GPB UGooglePlayBillingBPLibrary::QueryPurchaseHistory_OnSuccess;
FErrorDelegate_GPB UGooglePlayBillingBPLibrary::QueryPurchaseHistory_OnFailure;

void UGooglePlayBillingBPLibrary::QueryPurchaseHistory(
	const EProductType ProductType,
	FPurchasesListDelegate_GPB OnSuccess,
	FErrorDelegate_GPB OnFailure)
{
	UGooglePlayBillingBPLibrary::QueryPurchaseHistory_OnSuccess = OnSuccess;
	UGooglePlayBillingBPLibrary::QueryPurchaseHistory_OnFailure = OnFailure;

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Fetch Method ID
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_QueryPurchaseHistory", "(Ljava/lang/String;)V", false);
		// convert parameters to JNI types
		jstring jProductType = Env->NewStringUTF(ProductType == EProductType::INAPP ? "inapp" : "subs");
		// Call Void Method
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, jProductType);
		// Clean up local references
		Env->DeleteLocalRef(jProductType);
	}
#endif
}

/*
	Show In App Messages
*/
FOperationComplete_GPB UGooglePlayBillingBPLibrary::ShowInAppMessages_OnActionTaken;
FErrorDelegate_GPB UGooglePlayBillingBPLibrary::ShowInAppMessages_OnFailure;

void UGooglePlayBillingBPLibrary::ShowInAppMessages(
	FOperationComplete_GPB OnActionTaken,
	FErrorDelegate_GPB OnFailure)
{
	UGooglePlayBillingBPLibrary::ShowInAppMessages_OnActionTaken = OnActionTaken;
	UGooglePlayBillingBPLibrary::ShowInAppMessages_OnFailure = OnFailure;

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Fetch Method ID
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowInAppMessages", "()V", false);
		// Call Void Method
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
#endif
}

/*
	Is Feature Supported
*/
bool UGooglePlayBillingBPLibrary::IsFeatureSupported(
	const EFeatureType FeatureType)
{
	bool bIsSupported = false;

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Fetch Method ID
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsFeatureSupported", "(Ljava/lang/String;)Z", false);
		// convert parameters to JNI types
		auto FeatureTypeArg = FJavaHelper::ToJavaString(Env, UBillingJNIUtils::ConvertFeatureType(FeatureType));
		// Call boolean method
		jboolean IsSupportedJava = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method, *FeatureTypeArg);
		bIsSupported = (IsSupportedJava == JNI_TRUE);
	}
#endif

	return bIsSupported;
}

/*
	Is Ready
*/
bool UGooglePlayBillingBPLibrary::IsBillingReady()
{
	bool bIsReady = false;

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Fetch Method ID
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsBillingReady", "()Z", false);
		// Call boolean method
		jboolean IsSupportedJava = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method);
		bIsReady = (IsSupportedJava == JNI_TRUE);
	}
#endif

	return bIsReady;
}

/*
	Get Subscription Offer Token
*/
FString UGooglePlayBillingBPLibrary::GetSubscriptionOfferToken(
	TArray<FProductDetails> SubscriptionProducts,
	const FString& ProductId,
	const FString& BasePlanId,
	const FString& OfferId)
{
	FString OfferToken = TEXT("");

	for (FProductDetails ProductDetails: SubscriptionProducts)
	{
		if (ProductDetails.ProductId == ProductId)
		{
			for (FSubscriptionOfferDetails SubscriptionOfferDetails: ProductDetails.SubscriptionOfferDetails)
			{
				if (SubscriptionOfferDetails.BasePlanId == BasePlanId)
				{
					if (OfferId.IsEmpty() || SubscriptionOfferDetails.OfferId == OfferId)
					{
						return SubscriptionOfferDetails.OfferToken;
					}
				}
			}
		}
	}

	return OfferToken;
}

/*
	C++ Callback Function from JVM
*/
#if PLATFORM_ANDROID
JNI_METHOD void Java_com_porretgaming_googleplaybilling_GooglePlayBilling_CPPQueryProductDetails(JNIEnv* env, jclass clazz, jobjectArray productDetails)
{
	// Get the length of the Java array
	int32 numElements = env->GetArrayLength(productDetails);

	// Create a TArray<FProductDetails> to store the converted data
	TArray<FProductDetails> ProductDetailsArray;
	ProductDetailsArray.SetNum(numElements);
	for (int32 i = 0; i < numElements; i++)
	{
		// Get the Java ProductDetails object at the current index
		jobject javaProductDetails = env->GetObjectArrayElement(productDetails, i);
		ProductDetailsArray[i] = UBillingJNIUtils::BuildProductDetails(env, javaProductDetails);
	}

	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([ProductDetailsArray]()
		{
			UGooglePlayBillingBPLibrary::QueryProductDetails_OnSuccess.ExecuteIfBound(ProductDetailsArray);
		}),
		TStatId(), nullptr, ENamedThreads::GameThread);
}
JNI_METHOD void Java_com_porretgaming_googleplaybilling_GooglePlayBilling_CPPLaunchBillingFlow(JNIEnv* env, jclass clazz, jobject purchase)
{
	FPurchase Purchase = UBillingJNIUtils::BuildPurchase(env, purchase);

	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([Purchase]()
		{
			UGooglePlayBillingBPLibrary::LaunchBillingFlow_OnSuccess.ExecuteIfBound(Purchase);
		}),
		TStatId(), nullptr, ENamedThreads::GameThread);
}
JNI_METHOD void Java_com_porretgaming_googleplaybilling_GooglePlayBilling_CPPCheckForPurchases(JNIEnv* env, jclass clazz, jobject purchase)
{
	FPurchase Purchase = UBillingJNIUtils::BuildPurchase(env, purchase);

	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([Purchase]()
		{
			UGooglePlayBillingBPLibrary::CheckForPurchases_OnSuccess.ExecuteIfBound(Purchase);
		}),
		TStatId(), nullptr, ENamedThreads::GameThread);
}
JNI_METHOD void Java_com_porretgaming_googleplaybilling_GooglePlayBilling_CPPQueryPurchaseHistory(JNIEnv* env, jclass clazz, jobjectArray purchases)
{
	// Get the length of the Java array
	int32 numElements = env->GetArrayLength(purchases);

	// Create a TArray<FProductDetails> to store the converted data
	TArray<FPurchaseHistoryRecord> PurchasesArray;
	PurchasesArray.SetNum(numElements);
	for (int32 i = 0; i < numElements; i++)
	{
		// Get the Java ProductDetails object at the current index
		jobject javaPurchase = env->GetObjectArrayElement(purchases, i);
		PurchasesArray[i] = UBillingJNIUtils::BuildPurchaseHistory(env, javaPurchase);
	}

	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([PurchasesArray]()
		{
			UGooglePlayBillingBPLibrary::QueryPurchaseHistory_OnSuccess.ExecuteIfBound(PurchasesArray);
		}),
		TStatId(), nullptr, ENamedThreads::GameThread);
}
JNI_METHOD void Java_com_porretgaming_googleplaybilling_GooglePlayBilling_CPPShowInAppMessages(JNIEnv* env, jclass clazz)
{
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([]()
		{
			UGooglePlayBillingBPLibrary::ShowInAppMessages_OnActionTaken.ExecuteIfBound();
		}),
		TStatId(), nullptr, ENamedThreads::GameThread);
}
JNI_METHOD void Java_com_porretgaming_googleplaybilling_GooglePlayBilling_CPPBillingError(JNIEnv* env, jclass clazz, jint functionIndex, jint responseCode, jstring responseMessage)
{
	UGooglePlayBillingBPLibrary::CallAppropriateFailureDelegate(functionIndex, static_cast<int32>(responseCode), FJavaHelper::FStringFromLocalRef(env, responseMessage));
}
#endif

void UGooglePlayBillingBPLibrary::CallAppropriateFailureDelegate(int32 Index, int32 ResponseCode, FString ResponseMessage)
{
	EBillingResponseCode Error = static_cast<EBillingResponseCode>(ResponseCode);
	switch (Index)
	{
	case 1:
		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([Error, ResponseMessage]()
			{
				UGooglePlayBillingBPLibrary::QueryProductDetails_OnFailure.ExecuteIfBound(Error, ResponseMessage);
			}),
			TStatId(), nullptr, ENamedThreads::GameThread);
		break;
	case 2:
		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([Error, ResponseMessage]()
			{
				UGooglePlayBillingBPLibrary::LaunchBillingFlow_OnFailure.ExecuteIfBound(Error, ResponseMessage);
			}),
			TStatId(), nullptr, ENamedThreads::GameThread);
		break;
	case 3:
		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([Error, ResponseMessage]()
			{
				UGooglePlayBillingBPLibrary::CheckForPurchases_OnFailure.ExecuteIfBound(Error, ResponseMessage);
			}),
			TStatId(), nullptr, ENamedThreads::GameThread);
		break;
	case 4:
		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([Error, ResponseMessage]()
			{
				UGooglePlayBillingBPLibrary::QueryPurchaseHistory_OnFailure.ExecuteIfBound(Error, ResponseMessage);
			}),
			TStatId(), nullptr, ENamedThreads::GameThread);
		break;
	case 5:
		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([Error, ResponseMessage]()
			{
				UGooglePlayBillingBPLibrary::ShowInAppMessages_OnFailure.ExecuteIfBound(Error, ResponseMessage);
			}),
			TStatId(), nullptr, ENamedThreads::GameThread);
		break;
	}
}
