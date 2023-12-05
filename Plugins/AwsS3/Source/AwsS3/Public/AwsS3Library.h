/**
* Copyright (C) 2017-2023 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "AwsS3Types.h"
#include "Async/Future.h"
#include "AwsS3Library.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnS3Callback, UAwsS3ClientObject*, s3ClientObject);

UCLASS(BlueprintType)
class AWSS3_API UAwsS3ClientObject : public UObject
{
	GENERATED_BODY()

public:
	virtual ~UAwsS3ClientObject() override
	{
#if !DISABLE_S3
		if (m_S3ClientObject)
		{
			delete m_S3ClientObject;
			m_S3ClientObject = nullptr;
		}
#endif
	}

protected:
#if !DISABLE_S3
	Aws::S3::S3Client* m_S3ClientObject;
	Aws::Client::ClientConfiguration* m_ClientConfigurationObject{nullptr};
	Aws::Auth::AWSCredentials* m_CredentialsObject{nullptr};
#endif
	FAwsS3Credentials m_Credentials;
	FAwsS3ClientConfiguration m_ClientConfiguration;
	FOnS3Callback m_Callback;
	EPayloadSigningPolicy m_PayloadSigningPolicy;

public:
	void Activate();
#if !DISABLE_S3
	Aws::S3::S3Client* GetAwsS3Client() { return m_S3ClientObject; }
	Aws::S3::S3Client* GetAwsS3Client() const { return m_S3ClientObject; }
	TFuture<void> m_Future;
#endif

public:
	/**
	* Create a new S3 Client Object
	* @param: callback will be executed when the object has been created
	**/
	UFUNCTION(BlueprintCallable, Category = "AWS|S3", meta = (AutoCreateRefTerm = "Callback"))
	static void CreateAwsS3Object(FAwsS3Credentials Credentials, FAwsS3ClientConfiguration ClientConfiguration, EPayloadSigningPolicy PayloadSigningPolicy, const FOnS3Callback& Callback);
	UFUNCTION(BlueprintCallable, Category = "AWS|Client Object")
	void DestroyS3Object();
};

UCLASS()
class AWSS3_API US3Async : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override { Super::Activate(); }

	virtual void SetReadyToDestroy() override
	{
		Super::SetReadyToDestroy();
		RemoveFromRoot();
	}
};


UCLASS()
class AWSS3_API UAwsS3Library : public UObject
{
	GENERATED_BODY()

public:
	/** Return true if the directory exists. **/
	UFUNCTION(BlueprintCallable, Category = "AWS|S3|Utilities")
	static bool DirectoryExists(FString Directory);
	
	/** Create a directory and return true if the directory was created or already existed. **/
	UFUNCTION(BlueprintCallable, Category = "AWS|S3|Utilities")
	static bool CreateDirectory(FString Directory);
	
	/** Delete a directory and return true if the directory was deleted or otherwise does not exist. **/
	UFUNCTION(BlueprintCallable, Category = "AWS|S3|Utilities")
	static bool DeleteDirectory(FString Directory);
	
	/** Delete a file and return true if the file exists. Will not delete read only files. **/
	UFUNCTION(BlueprintCallable, Category = "AWS|S3|Utilities")
	static bool DeleteFile(FString Filename);
	
	/** Return true if the file is read only. **/
	UFUNCTION(BlueprintCallable, Category = "AWS|S3|Utilities")
	static bool IsReadOnly(FString Filename);
	
	/** Attempt to move a file. Return true if successful. Will not overwrite existing files. **/
	UFUNCTION(BlueprintCallable, Category = "AWS|S3|Utilities")
	static bool MoveFile(FString To, FString From);
	
	/** Attempt to change the read only status of a file. Return true if successful. **/
	UFUNCTION(BlueprintCallable, Category = "AWS|S3|Utilities")
	static bool SetReadOnly(FString Filename, bool bNewReadOnlyValue);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AWS|S3|Utilities")
	static int64 FileSizeOnDisk(FString File);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AWS|S3|Utilities", meta = (DisplayName = "ToString (int64)", CompactNodeTitle = "->", BlueprintAutocast))
	static FString Int64ToString(int64 Value);
};

UCLASS()
class AWSS3_API US3UUID : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AWS|S3", meta = (DisplayName = "AWS UUID"))
	static void AwsS3Uuid(FString& UUID);
};
