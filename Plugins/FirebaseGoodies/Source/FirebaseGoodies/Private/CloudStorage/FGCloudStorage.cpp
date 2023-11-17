// Copyright (c) 2023 Nineva Studios

#include "CloudStorage/FGCloudStorage.h"

#include "FirebaseGoodiesLog.h"
#include "CloudStorage/Interface/ICloudStorage.h"

#if PLATFORM_ANDROID
#include "Android/CloudStorageAndroid.h"
#endif

#if PLATFORM_IOS
#include "IOS/CloudStorageIOS.h"
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
#include "Desktop/CloudStorageDesktop.h"
#endif


#if (PLATFORM_WINDOWS || PLATFORM_MAC) && !FG_ENABLE_EDITOR_SUPPORT
TSharedPtr<ICloudStorage> UFGCloudStorage::CloudStorageImpl = nullptr;
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
TSharedPtr<ICloudStorage> UFGCloudStorage::CloudStorageImpl = MakeShareable(new CloudStorageDesktop());;
#endif

#if PLATFORM_ANDROID
TSharedPtr<ICloudStorage> UFGCloudStorage::CloudStorageImpl = MakeShareable(new CloudStorageAndroid());;
#endif

#if PLATFORM_IOS
TSharedPtr<ICloudStorage> UFGCloudStorage::CloudStorageImpl = MakeShareable(new CloudStorageIOS());;
#endif


void UFGCloudStorage::UploadFromDataInMemory(const TArray<uint8>& FileInMemory, const FString& FileStoragePath,
	const FCloudStorageIntDelegate& OnUploadFromDataInMemoryProgress,
	const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	if(!CloudStorageImpl)
		return;
	
	CloudStorageImpl->UploadFromDataInMemory(FileInMemory, FileStoragePath, OnUploadFromDataInMemoryProgress, OnSuccess, OnError);
}

void UFGCloudStorage::UploadFromLocalFile(const FString& LocalFilePath, const FString& FileStoragePath,
	const FCloudStorageIntDelegate& OnUploadProgress,
	const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	if(!CloudStorageImpl)
		return;
	
	CloudStorageImpl->UploadFromLocalFile(LocalFilePath, FileStoragePath, OnUploadProgress, OnSuccess, OnError);
}

void UFGCloudStorage::GetDownloadUrl(const FString& FileStoragePath, const FCloudStorageStringDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	if(!CloudStorageImpl)
		return;
	
	CloudStorageImpl->GetDownloadUrl(FileStoragePath, OnSuccess, OnError);
}

void UFGCloudStorage::DownloadToLocalFile(const FString& FileStoragePath, const FString& LocalFileName,
	EAndroidStorageLocation Destination,
	const FCloudStorageStringDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError, const FCloudStorageIntDelegate& OnProgress)
{
	if(!CloudStorageImpl)
		return;
	
	CloudStorageImpl->DownloadToLocalFile(FileStoragePath, LocalFileName, Destination, OnSuccess, OnError, OnProgress);
}

void UFGCloudStorage::DownloadInMemory(const FString& FileStoragePath, int64 FileSizeLimit,
	const FCloudStorageByteArrayDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	if(!CloudStorageImpl)
		return;
	
	CloudStorageImpl->DownloadInMemory(FileStoragePath, FileSizeLimit, OnSuccess, OnError);
}

void UFGCloudStorage::DeleteFile(const FString& FileStoragePath, const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	if(!CloudStorageImpl)
		return;
	
	CloudStorageImpl->DeleteFile(FileStoragePath, OnSuccess, OnError);
}

void UFGCloudStorage::GetFileMetadata(const FString& FileStoragePath, const FCloudStorageMetadataDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	if(!CloudStorageImpl)
		return;
	
	CloudStorageImpl->GetFileMetadata(FileStoragePath, OnSuccess, OnError);
}

void UFGCloudStorage::UpdateFileMetadata(const FString& FileStoragePath, UFGStorageMetadataValues* NewMetadataValues,
	const FCloudStorageMetadataDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	if(!CloudStorageImpl)
		return;
	
	CloudStorageImpl->UpdateFileMetadata(FileStoragePath, NewMetadataValues, OnSuccess, OnError);
}
