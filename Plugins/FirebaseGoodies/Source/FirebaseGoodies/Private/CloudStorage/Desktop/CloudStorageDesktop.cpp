// Copyright (c) 2023 Nineva Studios

#include "CloudStorageDesktop.h"

#include "CloudStorage/FGCloudStorage.h"
#include "CloudStorage/Callbacks/FGCloudStorageDeleteFileCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageDownloadInMemoryCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageDownloadUrlCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageFileMetadataCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageProgressCallback.h"
#include "CloudStorage/Callbacks/FGCloudStorageUploadCallback.h"

#include "FirebaseGoodiesLog.h"

#include "Async/Async.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT

#include "firebase/app.h"
#include "firebase/storage.h"

#include "Desktop/Storage/FGStorageOperationListener.h"
#include "Desktop/Storage/FGDownloadStorageOperationListener.h"
#include "Desktop/FGUtils.h"

CloudStorageDesktop::~CloudStorageDesktop()
{
}

void CloudStorageDesktop::UploadFromDataInMemory(const TArray<uint8>& FileInMemory, const FString& FileStoragePath,
	const FCloudStorageIntDelegate& OnUploadFromDataInMemoryProgress, const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageUploadCallback* NativeCallback = NewObject<UFGCloudStorageUploadCallback>();
	NativeCallback->BindProgressDelegate(OnUploadFromDataInMemoryProgress);
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	const int32 BufferSize = FileInMemory.Num();

	firebase::storage::Storage* storage = firebase::storage::Storage::GetInstance(firebase::App::GetInstance());

	firebase::storage::StorageReference reference = storage->GetReference(FGUtils::GetStringCopy(FileStoragePath));

	FGStorageOperationListener listener(NativeCallback);

	auto task = reference.PutBytes(FileInMemory.GetData(), BufferSize, &listener);
	task.AddOnCompletion([=](const ::firebase::Future<firebase::storage::Metadata>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			NativeCallback->ExecuteSuccess();
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Upload From Data In Memory error: %s"), ErrorMessage.IsEmpty() ? TEXT("Empty error message") : *ErrorMessage);

			NativeCallback->ExecuteError(ErrorMessage);
		}

		FGUtils::ClearStringCopies();

	});
}

void CloudStorageDesktop::UploadFromLocalFile(const FString& LocalFilePath, const FString& FileStoragePath,
	const FCloudStorageIntDelegate& OnUploadProgress, const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageUploadCallback* NativeCallback = NewObject<UFGCloudStorageUploadCallback>();
	NativeCallback->BindProgressDelegate(OnUploadProgress);
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	firebase::storage::Storage* storage = firebase::storage::Storage::GetInstance(firebase::App::GetInstance());

	firebase::storage::StorageReference reference = storage->GetReference(FGUtils::GetStringCopy(FileStoragePath));

	FGStorageOperationListener listener(NativeCallback);

	auto task = reference.PutFile(FGUtils::GetStringCopy(LocalFilePath), &listener);
	task.AddOnCompletion([=](const ::firebase::Future<firebase::storage::Metadata>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			NativeCallback->ExecuteSuccess();
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Upload From File error: %s"), ErrorMessage.IsEmpty() ? TEXT("Empty error message") : *ErrorMessage);

			NativeCallback->ExecuteError(ErrorMessage);
		}

		FGUtils::ClearStringCopies();
		
	});
}

void CloudStorageDesktop::GetDownloadUrl(const FString& FileStoragePath, const FCloudStorageStringDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	FString downloadUrl;
	UFGCloudStorageDownloadUrlCallback* NativeCallback = NewObject<UFGCloudStorageDownloadUrlCallback>();

	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	firebase::storage::Storage* storage = firebase::storage::Storage::GetInstance(firebase::App::GetInstance());

	firebase::storage::StorageReference reference = storage->GetReference(FGUtils::GetStringCopy(FileStoragePath));

	auto task = reference.GetDownloadUrl();
	task.AddOnCompletion([=](const ::firebase::Future<std::string>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			FString Url = (*callback.result()).c_str();

			UE_LOG(LogFirebaseGoodies, Verbose, TEXT("Get Download Url: %s"), *Url);

			NativeCallback->ExecuteSuccess(Url);
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Get Download Url error: %s"), ErrorMessage.IsEmpty() ? TEXT("Empty error message") : *ErrorMessage);

			NativeCallback->ExecuteError(ErrorMessage);
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudStorageDesktop::DownloadToLocalFile(const FString& FileStoragePath, const FString& LocalFileName,
	EAndroidStorageLocation Destination, const FCloudStorageStringDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError, const FCloudStorageIntDelegate& OnProgress)
{
	FString EnumAsString = UEnum::GetDisplayValueAsText(Destination).ToString();

	UFGCloudStorageProgressCallback* NativeCallback = NewObject<UFGCloudStorageProgressCallback>();
	
	NativeCallback->BindProgressDelegate(OnProgress);
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	firebase::storage::Storage* storage = firebase::storage::Storage::GetInstance(firebase::App::GetInstance());

	firebase::storage::StorageReference reference = storage->GetReference(FGUtils::GetStringCopy(FileStoragePath));

	FGDownloadStorageOperationListener listener(NativeCallback);

	FString FilePath = FPaths::Combine(FPaths::ProjectDir(), LocalFileName);
	FString FullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FilePath);

	UE_LOG(LogFirebaseGoodies, Verbose, TEXT("Downloading file to: %s"), *FullPath);

	auto task = reference.GetFile(FGUtils::GetStringCopy(FullPath), &listener);
	task.AddOnCompletion([=](const ::firebase::Future<size_t>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			NativeCallback->ExecuteSuccess(FullPath);
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Download To Local File error: %s"), ErrorMessage.IsEmpty() ? TEXT("Empty error message") : *ErrorMessage);

			NativeCallback->ExecuteError(ErrorMessage);
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudStorageDesktop::DownloadInMemory(const FString& FileStoragePath, int64 FileSizeLimit,
	const FCloudStorageByteArrayDelegate& OnSuccess, const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageDownloadInMemoryCallback* NativeCallback = NewObject<UFGCloudStorageDownloadInMemoryCallback>();

	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);
	firebase::storage::Storage* storage = firebase::storage::Storage::GetInstance(firebase::App::GetInstance());

	firebase::storage::StorageReference reference = storage->GetReference(FGUtils::GetStringCopy(FileStoragePath));

	uint8* buffer = new uint8[FileSizeLimit];

	auto task = reference.GetBytes(buffer, FileSizeLimit);
	task.AddOnCompletion([=](const ::firebase::Future<size_t>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			size_t Size = *callback.result();
			TArray<uint8> byteArray = TArray<uint8>(buffer, FGenericPlatformMath::Min(FileSizeLimit, (int64) Size));

			NativeCallback->ExecuteSuccess(byteArray);
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Download In Memory error: %s"), ErrorMessage.IsEmpty() ? TEXT("Empty error message") : *ErrorMessage);

			NativeCallback->ExecuteError(ErrorMessage);
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudStorageDesktop::DeleteFile(const FString& FileStoragePath, const FCloudStorageVoidDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageDeleteFileCallback* NativeCallback = NewObject<UFGCloudStorageDeleteFileCallback>();

	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);
	
	firebase::storage::Storage* storage = firebase::storage::Storage::GetInstance(firebase::App::GetInstance());

	firebase::storage::StorageReference reference = storage->GetReference(FGUtils::GetStringCopy(FileStoragePath));

	auto task = reference.Delete();
	task.AddOnCompletion([=](const ::firebase::Future<void>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			NativeCallback->ExecuteSuccess();
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Delete File error: %s"), ErrorMessage.IsEmpty() ? TEXT("Empty error message") : *ErrorMessage);

			NativeCallback->ExecuteError(ErrorMessage);
		}

		FGUtils::ClearStringCopies();
	});
}


void CloudStorageDesktop::GetFileMetadata(const FString& FileStoragePath,
	const FCloudStorageMetadataDelegate& OnSuccess, const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageFileMetadataCallback* NativeCallback = NewObject<UFGCloudStorageFileMetadataCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	firebase::storage::Storage* storage = firebase::storage::Storage::GetInstance(firebase::App::GetInstance());

	firebase::storage::StorageReference reference = storage->GetReference(FGUtils::GetStringCopy(FileStoragePath));

	auto task = reference.GetMetadata();
	task.AddOnCompletion([=](const ::firebase::Future<firebase::storage::Metadata>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			UFGStorageMetadata* result = NewObject<UFGStorageMetadata>();
			result->Init(*callback.result());

			NativeCallback->ExecuteSuccess(result);
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Get File Metadata error: %s"), ErrorMessage.IsEmpty() ? TEXT("Empty error message") : *ErrorMessage);

			NativeCallback->ExecuteError(ErrorMessage);
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudStorageDesktop::UpdateFileMetadata(const FString& FileStoragePath,
	UFGStorageMetadataValues* NewMetadataValues, const FCloudStorageMetadataDelegate& OnSuccess,
	const FCloudStorageStringDelegate& OnError)
{
	UFGCloudStorageFileMetadataCallback* NativeCallback = NewObject<UFGCloudStorageFileMetadataCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	firebase::storage::Storage* storage = firebase::storage::Storage::GetInstance(firebase::App::GetInstance());

	firebase::storage::StorageReference reference = storage->GetReference(FGUtils::GetStringCopy(FileStoragePath));

	auto task = reference.UpdateMetadata(NewMetadataValues->desktopMetadata);
	task.AddOnCompletion([=](const ::firebase::Future<firebase::storage::Metadata>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			UFGStorageMetadata* result = NewObject<UFGStorageMetadata>();
			result->Init(*callback.result());

			NativeCallback->ExecuteSuccess(result);
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Update File Metadata error: %s"), ErrorMessage.IsEmpty() ? TEXT("Empty error message") : *ErrorMessage);

			NativeCallback->ExecuteError(ErrorMessage);

		}
	});
}

#endif