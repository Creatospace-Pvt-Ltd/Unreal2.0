#include "CloudStorageIOS.h"

#include "Async/Async.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "FirebaseGoodiesLog.h"
#include "CloudStorage/FGCloudStorage.h"

#import <FirebaseStorage/FirebaseStorage.h>

#include "CloudStorage/Callbacks/FGCloudStorageDeleteFileCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageDownloadInMemoryCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageDownloadUrlCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageFileMetadataCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageProgressCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageUploadCallback.h"


CloudStorageIOS::~CloudStorageIOS()
{
	
}

void CloudStorageIOS::UploadFromDataInMemory(const TArray<uint8>& FileInMemory, const FString& FileStoragePath,
	const FCloudStorageIntDelegate& OnUploadFromDataInMemoryProgress, const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageUploadCallback* NativeCallback = NewObject<UFGCloudStorageUploadCallback>();
	NativeCallback->BindProgressDelegate(OnUploadFromDataInMemoryProgress);
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);
	
	const int32 BufferSize = FileInMemory.Num();
	
	FIRStorage* storage = [FIRStorage storage];
	FIRStorageReference* storageRef = [storage reference];
	FIRStorageReference* storageFileRef = [storageRef child:FileStoragePath.GetNSString()];

	uint8* byteData = (uint8*)malloc(BufferSize);
	for (int32 i = 0; i < BufferSize; i++)
	{
		byteData[i] = FileInMemory[i];
	}

	NSData* data = [NSData dataWithBytes:byteData length:BufferSize];

	FIRStorageUploadTask* uploadTask = [storageFileRef putData:data metadata:nil completion:^(FIRStorageMetadata* metadata, NSError* error) {
		if (error != nil)
		{
			FString ErrorMessage = FString(error.localizedDescription);
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to upload: %s"), * ErrorMessage);
			NativeCallback->ExecuteError(ErrorMessage);
		}
		else
		{
			NativeCallback->ExecuteSuccess();
		}
	}];
	
	FIRStorageHandle observer = [uploadTask observeStatus:FIRStorageTaskStatusProgress handler:^(FIRStorageTaskSnapshot *snapshot) {
		int progress = (int) (snapshot.progress.fractionCompleted * 100);
		NativeCallback->ExecuteProgress(progress);
	}];
}

void CloudStorageIOS::UploadFromLocalFile(const FString& LocalFilePath, const FString& FileStoragePath,
	const FCloudStorageIntDelegate& OnUploadProgress, const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageUploadCallback* NativeCallback = NewObject<UFGCloudStorageUploadCallback>();
	NativeCallback->BindProgressDelegate(OnUploadProgress);
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	FIRStorage* storage = [FIRStorage storage];
	FIRStorageReference* storageRef = [storage reference];
	FIRStorageReference* storageFileRef = [storageRef child:FileStoragePath.GetNSString()];

	NSData* data = [NSData dataWithContentsOfFile:LocalFilePath.GetNSString()];

	FIRStorageUploadTask* uploadTask = [storageFileRef putData:data metadata:nil completion:^(FIRStorageMetadata* metadata, NSError* error) {
		if (error != nil)
		{
			FString ErrorMessage = FString(error.localizedDescription);
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to upload: %s"), *ErrorMessage);
			NativeCallback->ExecuteError(ErrorMessage);
		}
		else
		{
			NativeCallback->ExecuteSuccess();
		}
	}];
	
	FIRStorageHandle observer = [uploadTask observeStatus:FIRStorageTaskStatusProgress handler:^(FIRStorageTaskSnapshot *snapshot) {
		int progress = (int) (snapshot.progress.fractionCompleted * 100);
		
		NativeCallback->ExecuteProgress(progress);
	}];
}

void CloudStorageIOS::GetDownloadUrl(const FString& FileStoragePath, const FCloudStorageStringDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageDownloadUrlCallback* NativeCallback = NewObject<UFGCloudStorageDownloadUrlCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	FIRStorage* storage = [FIRStorage storage];
	FIRStorageReference* storageRef = [storage reference];
	FIRStorageReference* storageFileRef = [storageRef child:FileStoragePath.GetNSString()];

	[storageFileRef downloadURLWithCompletion:^(NSURL* url, NSError* error) {
		if (error != nil)
		{
			FString ErrorMessage = FString(error.localizedDescription);
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to download: %s"), *ErrorMessage);
			NativeCallback->ExecuteError(ErrorMessage);
		}
		else
		{
			FString result = FString(url.absoluteString);
			NativeCallback->ExecuteSuccess(result);
		}
	}];
}

void CloudStorageIOS::DownloadToLocalFile(const FString& FileStoragePath, const FString& LocalFileName,
	EAndroidStorageLocation Destination, const FCloudStorageStringDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError, const FCloudStorageIntDelegate& OnProgress)
{
	UFGCloudStorageProgressCallback* NativeCallback = NewObject<UFGCloudStorageProgressCallback>();
	NativeCallback->BindProgressDelegate(OnProgress);
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	FIRStorage* storage = [FIRStorage storage];
	FIRStorageReference* storageRef = [storage reference];
	FIRStorageReference* fileRef = [storageRef child:FileStoragePath.GetNSString()];

	NSString* documentPath = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject].absoluteString;
	NSString* filePath = [documentPath stringByAppendingPathComponent:LocalFileName.GetNSString()];

	UE_LOG(LogFirebaseGoodies, Log, TEXT("Downloading file to: %s"), *FString(filePath));

	NSURL* localURL = [NSURL URLWithString:filePath];

	FIRStorageDownloadTask* downloadTask = [fileRef writeToFile:localURL completion:^(NSURL* URL, NSError* error) {
		if (error != nil)
		{
			FString ErrorMessage = FString(error.localizedDescription);
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to download: %s"), *ErrorMessage);
			NativeCallback->ExecuteError(ErrorMessage);
		}
		else
		{
			NSURL* fileUrl = (NSURL*)CFBridgingRetain(URL);
			
			NSString *fullPath = fileUrl.absoluteString;
			NSString *prefixToRemove = @"file:";
			if ([fullPath hasPrefix:prefixToRemove]) {
				fullPath = [fullPath substringFromIndex:[prefixToRemove length]];
			}
			
			NativeCallback->ExecuteSuccess(FString(fullPath));
			CFBridgingRelease(fileUrl);}
	}];
		
	FIRStorageHandle observer = [downloadTask observeStatus:FIRStorageTaskStatusProgress handler:^(FIRStorageTaskSnapshot *snapshot) {
		int progress = (int) (snapshot.progress.fractionCompleted * 100);		
		NativeCallback->ExecuteProgress(progress);
	}];
}

void CloudStorageIOS::DeleteFile(const FString& FileStoragePath, const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageDeleteFileCallback* NativeCallback = NewObject<UFGCloudStorageDeleteFileCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	FIRStorage* storage = [FIRStorage storage];
	FIRStorageReference* storageRef = [storage reference];
	FIRStorageReference* fileRef = [storageRef child:FileStoragePath.GetNSString()];

	[fileRef deleteWithCompletion:^(NSError* error) {
		if (error != nil)
		{
			FString ErrorMessage = FString(error.localizedDescription);
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to delete: %s"), *ErrorMessage);
			NativeCallback->ExecuteError(ErrorMessage);
		}
		else
		{
			NativeCallback->ExecuteSuccess();
		}
	}];
}

void CloudStorageIOS::DownloadInMemory(const FString& FileStoragePath, int64 FileSizeLimit,
	const FCloudStorageByteArrayDelegate& OnSuccess, const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageDownloadInMemoryCallback* NativeCallback = NewObject<UFGCloudStorageDownloadInMemoryCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	FIRStorage* storage = [FIRStorage storage];
	FIRStorageReference* storageRef = [storage reference];
	FIRStorageReference* fileRef = [storageRef child:FileStoragePath.GetNSString()];

	[fileRef dataWithMaxSize:(int64)FileSizeLimit completion:^(NSData* data, NSError* error) {
		if (error != nil)
		{
			FString ErrorMessage = FString(error.localizedDescription);
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to download: %s"), *ErrorMessage);
			NativeCallback->ExecuteError(ErrorMessage);
		}
		else
		{
			NSUInteger len = [data length];
			uint8* byteData = (uint8*)[data bytes];

			TArray<uint8> byteArray;
			byteArray.SetNumZeroed(len);

			for (int32 i = 0; i < len; i++)
			{
				byteArray[i] = byteData[i];
			}
			NativeCallback->ExecuteSuccess(byteArray);
		}
	}];
}

void CloudStorageIOS::GetFileMetadata(const FString& FileStoragePath, const FCloudStorageMetadataDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageFileMetadataCallback* NativeCallback = NewObject<UFGCloudStorageFileMetadataCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	FIRStorage* storage = [FIRStorage storage];
	FIRStorageReference* storageRef = [storage reference];
	FIRStorageReference* fileRef = [storageRef child:FileStoragePath.GetNSString()];

	[fileRef metadataWithCompletion:^(FIRStorageMetadata* metadata, NSError* error) {
		if (error != nil)
		{
			FString ErrorMessage = FString(error.localizedDescription);
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to get metadata: %s"), *ErrorMessage);
			NativeCallback->ExecuteError(ErrorMessage);
		}
		else
		{
			UFGStorageMetadata* result = NewObject<UFGStorageMetadata>();
			result->Init(metadata);

			NativeCallback->ExecuteSuccess(result);
		}
	}];
}

void CloudStorageIOS::UpdateFileMetadata(const FString& FileStoragePath, UFGStorageMetadataValues* NewMetadataValues,
	const FCloudStorageMetadataDelegate& OnSuccess, const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageFileMetadataCallback* NativeCallback = NewObject<UFGCloudStorageFileMetadataCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	FIRStorage* storage = [FIRStorage storage];
	FIRStorageReference* storageRef = [storage reference];
	FIRStorageReference* fileRef = [storageRef child:FileStoragePath.GetNSString()];

	[fileRef updateMetadata:NewMetadataValues->iosMetadata
				 completion:^(FIRStorageMetadata* metadata, NSError* error) {
					 if (error != nil)
					 {
					 	 FString ErrorMessage = FString(error.localizedDescription);
					 	 UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to update metadata: %s"), *ErrorMessage);
					 	 NativeCallback->ExecuteError(ErrorMessage);

					 }
					 else
					 {
					 	 UFGStorageMetadata* result = NewObject<UFGStorageMetadata>();
						 result->Init(metadata);

					 	NativeCallback->ExecuteSuccess(result);
					 }
				 }];
}