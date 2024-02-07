#include "AndroidDeepLinkFunctions.h"

#if PLATFORM_ANDROID

#include <Android/AndroidJNI.h>
#include <Android/AndroidApplication.h>
//#include <Launch/Public/Android/AndroidJNI.h>

#define INIT_JAVA_METHOD(name, signature) \
if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true)) { \
	name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false); \
	check(name != NULL); \
} else { \
	check(0); \
}

#define DECLARE_JAVA_METHOD(name) \
static jmethodID name = NULL;

DECLARE_JAVA_METHOD(AndroidThunkJava_GetDeepLink);		// Here goes the name of the method in the Java side

void UAndroidDeepLinkFunctions::InitJavaFunctions()
{
	// Same here, but we add the Java signature (the type of the parameters is between the parameters, and the return type is added at the end,
	// here the return type is V for "void")
	// More details here about Java signatures: http://www.rgagnon.com/javadetails/java-0286.html
	INIT_JAVA_METHOD(AndroidThunkJava_GetDeepLink, "(Ljava/lang/String;)V");
}
#undef DECLARE_JAVA_METHOD
#undef INIT_JAVA_METHOD

// Declare the native function
extern "C" void Java_com_epicgames_unreal_GameActivity_AndroidGetDeepLink(JNIEnv * env, jobject thiz, jstring deepLink)
{
	// Convert Java string to C++ FString
	const char* deepLinkChars = env->GetStringUTFChars(deepLink, nullptr);
	FString deepLinkValue(deepLinkChars);

	// Release resources
	env->ReleaseStringUTFChars(deepLink, deepLinkChars);
}

#endif



FString UAndroidDeepLinkFunctions::AndroidGetDeepLink() {
    
#if PLATFORM_ANDROID
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();

    // Find the GameActivity class
    jclass ActivityClass = FAndroidApplication::FindJavaClass("com/epicgames/unreal/GameActivity");

    if (ActivityClass)
    {
        // Find the getStoredDeepLink method
        jmethodID MethodGetStoredDeepLink = Env->GetStaticMethodID(ActivityClass, "AndroidGetDeepLink", "()Ljava/lang/String;");

        if (MethodGetStoredDeepLink)
        {
            // Call the method to get the stored deep link
            jstring storedDeepLinkObject = (jstring)Env->CallObjectMethod(ActivityClass, MethodGetStoredDeepLink);

            if (storedDeepLinkObject)
            {
                // Convert the Java string to C++ FString
                const char* storedDeepLinkChars = Env->GetStringUTFChars(storedDeepLinkObject, nullptr);
                FString storedDeepLinkValue(storedDeepLinkChars);

                // Release resources
                Env->ReleaseStringUTFChars(storedDeepLinkObject, storedDeepLinkChars);
                Env->DeleteLocalRef(storedDeepLinkObject);

                return storedDeepLinkValue;
            }
            else return ("No Obj");
        }
        else return ("No Method");
    }

    return FString("Failed to get stored deep link");
#endif

    return FString("Not Android");
}