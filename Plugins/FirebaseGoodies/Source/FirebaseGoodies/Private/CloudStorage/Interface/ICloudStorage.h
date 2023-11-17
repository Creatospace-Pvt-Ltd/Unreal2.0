// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudStorage/FGCloudStorage.h"


class ICloudStorage
{
public:
	virtual ~ICloudStorage() = default;
	virtual void UploadFromDataInMemory(const TArray<uint8>& FileInMemory, const FString& FileStoragePath,
	                                    const FCloudStorageIntDelegate& OnUploadFromDataInMemoryProgress,
	                                    const FCloudStorageVoidDelegate& OnSuccess,
	                                    const FCloudStorageStringDelegate& OnError) = 0;
	
	virtual void UploadFromLocalFile(const FString& LocalFilePath, const FString& FileStoragePath,
										const FCloudStorageIntDelegate& OnUploadProgress,
										const FCloudStorageVoidDelegate& OnSuccess,
										const FCloudStorageStringDelegate& OnError) = 0;
	
	virtual void GetDownloadUrl(const FString& FileStoragePath, const FCloudStorageStringDelegate& OnSuccess,
										const FCloudStorageStringDelegate& OnError) = 0;
	
	virtual void DownloadToLocalFile(const FString& FileStoragePath, const FString& LocalFileName,
										EAndroidStorageLocation Destination,
										const FCloudStorageStringDelegate& OnSuccess,
										const FCloudStorageStringDelegate& OnError,
										const FCloudStorageIntDelegate& OnProgress) = 0;
	
	virtual void DeleteFile(const FString& FileStoragePath, const FCloudStorageVoidDelegate& OnSuccess,
										const FCloudStorageStringDelegate& OnError) = 0;
	
	virtual void DownloadInMemory(const FString& FileStoragePath, int64 FileSizeLimit,
										const FCloudStorageByteArrayDelegate& OnSuccess,
										const FCloudStorageStringDelegate& OnError) = 0;
	
	virtual void GetFileMetadata(const FString& FileStoragePath, const FCloudStorageMetadataDelegate& OnSuccess,
										const FCloudStorageStringDelegate& OnError) = 0;
	
	virtual void UpdateFileMetadata(const FString& FileStoragePath, UFGStorageMetadataValues* NewMetadataValues,
										const FCloudStorageMetadataDelegate& OnSuccess,
										const FCloudStorageStringDelegate& OnError) = 0;
};