/**
* Copyright (C) 2017-2023 eelDev AB
*/

#include "AwsS3Async.h"
#include "AwsS3Logging.h"
#include "AwsS3PrivatePCH.h"
#include <string>
#include <sstream>
#include <iostream>

#include "Async/TaskGraphInterfaces.h"

US3GetObject* US3GetObject::GetObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetObject>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetObject::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetObjectRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.IfMatch.IsEmpty() == false)
		Request.SetIfMatch(TCHAR_TO_UTF8(*m_RequestData.IfMatch));

	if (m_RequestData.IfModifiedSince > 0)
		Request.SetIfModifiedSince(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.IfModifiedSince.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.IfNoneMatch.IsEmpty() == false)
		Request.SetIfNoneMatch(TCHAR_TO_UTF8(*m_RequestData.IfNoneMatch));

	if (m_RequestData.IfUnmodifiedSince > 0)
		Request.SetIfUnmodifiedSince(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.IfUnmodifiedSince.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.Range.IsEmpty() == false)
		Request.SetRange(TCHAR_TO_UTF8(*m_RequestData.Range));

	if (m_RequestData.ResponseCacheControl.IsEmpty() == false)
		Request.SetResponseCacheControl(TCHAR_TO_UTF8(*m_RequestData.ResponseCacheControl));

	if (m_RequestData.ResponseContentDisposition.IsEmpty() == false)
		Request.SetResponseContentDisposition(TCHAR_TO_UTF8(*m_RequestData.ResponseContentDisposition));

	if (m_RequestData.ResponseContentEncoding.IsEmpty() == false)
		Request.SetResponseContentEncoding(TCHAR_TO_UTF8(*m_RequestData.ResponseContentEncoding));

	if (m_RequestData.ResponseContentLanguage.IsEmpty() == false)
		Request.SetResponseContentLanguage(TCHAR_TO_UTF8(*m_RequestData.ResponseContentLanguage));

	if (m_RequestData.ResponseContentType.IsEmpty() == false)
		Request.SetResponseContentType(TCHAR_TO_UTF8(*m_RequestData.ResponseContentType));

	if (m_RequestData.ResponseExpires > 0)
		Request.SetResponseExpires(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.ResponseExpires.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	if (m_RequestData.SSECustomerAlgorithm.IsEmpty() == false)
		Request.SetSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.SSECustomerAlgorithm));

	if (m_RequestData.SSECustomerKey.IsEmpty() == false)
		Request.SetSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKey));

	if (m_RequestData.SSECustomerKeyMD5.IsEmpty() == false)
		Request.SetSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKeyMD5));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.PartNumber > 0)
		Request.SetPartNumber(m_RequestData.PartNumber);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetObjectRequest&, Aws::S3::Model::GetObjectOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		bool bIsSuccess = outcome.IsSuccess();
		auto& Outcome = outcome;
		auto OutcomeResult = outcome.GetResultWithOwnership();

		FGetObjectResult Result = OutcomeResult;

		ES3Errors Error = ES3Errors::UNKNOWN;
		FString ErrorMessage;

		if (bIsSuccess)
		{
			FString FinalFileName = FString::Printf(TEXT("%s/%s"), *m_RequestData.FileSavePath, m_RequestData.CustomFileName.IsEmpty() == false ? *m_RequestData.CustomFileName : *m_RequestData.Key);

			LogVerbose("Final File Name: %s", *FinalFileName);
			
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			int64 FileSizeOnDisk = PlatformFile.FileSize(*FinalFileName);
			uint64 SizeOnDisk = (FileSizeOnDisk > 0) ? static_cast<uint64>(FileSizeOnDisk) : 0;

			if (SizeOnDisk > 0)
			{
				LogVerbose("File (%s) already exists. SizeOnDisk: %llu", *FinalFileName, SizeOnDisk);
			}
			
			IFileHandle* File = PlatformFile.OpenWrite(*FinalFileName, SizeOnDisk > 0, true);

			if (File)
			{
				int64 FileSize = OutcomeResult.GetContentLength();
				LogVerbose("Content Range: %s", UTF8_TO_TCHAR(OutcomeResult.GetContentRange().c_str()));
				LogVerbose("File Content Length: %llu", FileSize);
				uint8* FileData = new uint8[FileSize];
				auto& Body = OutcomeResult.GetBody();
				Body.read((char*)FileData, FileSize);

				if (File->Write(FileData, FileSize))
				{
					LogVerbose("Wrote (%d) bytes to file (%s)", FileSize, *FinalFileName);
				}
				else
				{
					LogError("Failed when writing (%d bytes) to %s", FileSize, *FinalFileName);	
				}

				delete File;
				delete[] FileData;
			}
			else
			{
				bIsSuccess = false;
				
				ErrorMessage = FString::Printf(TEXT("Could not write file (%s)"), *FinalFileName);
				
				LogError("Could not write file (%s)", *FinalFileName);
			}
		}
		else
		{
			Error = static_cast<ES3Errors>(Outcome.GetError().GetErrorType());
			ErrorMessage = UTF8_TO_TCHAR(Outcome.GetError().GetMessage().c_str());
		}

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(bIsSuccess, Result, Error, ErrorMessage);
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetObjectAsync(Request, CallbackLambda);
#endif
}

US3AbortMultipartUpload* US3AbortMultipartUpload::AbortMultipartUpload(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FAbortMultipartUploadRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3AbortMultipartUpload>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3AbortMultipartUpload::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::AbortMultipartUploadRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.UploadId.IsEmpty() == false)
		Request.SetUploadId(TCHAR_TO_UTF8(*m_RequestData.UploadId));

	Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::AbortMultipartUploadRequest&, Aws::S3::Model::AbortMultipartUploadOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->AbortMultipartUploadAsync(Request, CallbackLambda);
#endif
}

US3CompleteMultipartUpload* US3CompleteMultipartUpload::CompleteMultipartUpload(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FCompleteMultipartUploadRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3CompleteMultipartUpload>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3CompleteMultipartUpload::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::CompleteMultipartUploadRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	Request.SetMultipartUpload(m_RequestData.MultipartUpload);

	if (m_RequestData.UploadId.IsEmpty() == false)
		Request.SetUploadId(TCHAR_TO_UTF8(*m_RequestData.UploadId));

	Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::CompleteMultipartUploadRequest&, Aws::S3::Model::CompleteMultipartUploadOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->CompleteMultipartUploadAsync(Request, CallbackLambda);
#endif
}

US3CopyObject* US3CopyObject::CopyObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FCopyObjectRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3CopyObject>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3CopyObject::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::CopyObjectRequest Request = {};

	if (m_RequestData.ACL != EObjectCannedACL::NOT_SET)
		Request.SetACL(static_cast<Aws::S3::Model::ObjectCannedACL>(m_RequestData.ACL));

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.CacheControl.IsEmpty() == false)
		Request.SetCacheControl(TCHAR_TO_UTF8(*m_RequestData.CacheControl));

	if (m_RequestData.ContentDisposition.IsEmpty() == false)
		Request.SetContentDisposition(TCHAR_TO_UTF8(*m_RequestData.ContentDisposition));

	if (m_RequestData.ContentEncoding.IsEmpty() == false)
		Request.SetContentEncoding(TCHAR_TO_UTF8(*m_RequestData.ContentEncoding));

	if (m_RequestData.ContentLanguage.IsEmpty() == false)
		Request.SetContentLanguage(TCHAR_TO_UTF8(*m_RequestData.ContentLanguage));

	if (m_RequestData.ContentType.IsEmpty() == false)
		Request.SetContentType(TCHAR_TO_UTF8(*m_RequestData.ContentType));

	if (m_RequestData.CopySource.IsEmpty() == false)
		Request.SetCopySource(TCHAR_TO_UTF8(*m_RequestData.CopySource));

	if (m_RequestData.CopySourceIfMatch.IsEmpty() == false)
		Request.SetCopySourceIfMatch(TCHAR_TO_UTF8(*m_RequestData.CopySourceIfMatch));

	if (m_RequestData.CopySourceIfModifiedSince > 0)
		Request.SetCopySourceIfModifiedSince(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.CopySourceIfModifiedSince.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.CopySourceIfNoneMatch.IsEmpty() == false)
		Request.SetCopySourceIfNoneMatch(TCHAR_TO_UTF8(*m_RequestData.CopySourceIfNoneMatch));

	if (m_RequestData.CopySourceIfUnmodifiedSince > 0)
		Request.SetCopySourceIfUnmodifiedSince(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.CopySourceIfUnmodifiedSince.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.Expires > 0)
		Request.SetExpires(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.Expires.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.GrantFullControl.IsEmpty() == false)
		Request.SetGrantFullControl(TCHAR_TO_UTF8(*m_RequestData.GrantFullControl));

	if (m_RequestData.GrantRead.IsEmpty() == false)
		Request.SetGrantRead(TCHAR_TO_UTF8(*m_RequestData.GrantRead));

	if (m_RequestData.GrantReadACP.IsEmpty() == false)
		Request.SetGrantReadACP(TCHAR_TO_UTF8(*m_RequestData.GrantReadACP));

	if (m_RequestData.GrantWriteACP.IsEmpty() == false)
		Request.SetGrantWriteACP(TCHAR_TO_UTF8(*m_RequestData.GrantWriteACP));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	for (auto& Element : m_RequestData.Metadata)
		Request.AddMetadata(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	if (m_RequestData.MetadataDirective != EMetadataDirective::NOT_SET)
		Request.SetMetadataDirective(static_cast<Aws::S3::Model::MetadataDirective>(m_RequestData.MetadataDirective));

	if (m_RequestData.TaggingDirective != ETaggingDirective::NOT_SET)
		Request.SetTaggingDirective(static_cast<Aws::S3::Model::TaggingDirective>(m_RequestData.TaggingDirective));

	if (m_RequestData.ServerSideEncryption != EServerSideEncryption::NOT_SET)
		Request.SetServerSideEncryption(static_cast<Aws::S3::Model::ServerSideEncryption>(m_RequestData.ServerSideEncryption));

	if (m_RequestData.StorageClass != EStorageClass::NOT_SET)
		Request.SetStorageClass(static_cast<Aws::S3::Model::StorageClass>(m_RequestData.StorageClass));

	if (m_RequestData.WebsiteRedirectLocation.IsEmpty() == false)
		Request.SetWebsiteRedirectLocation(TCHAR_TO_UTF8(*m_RequestData.WebsiteRedirectLocation));

	if (m_RequestData.SSECustomerAlgorithm.IsEmpty() == false)
		Request.SetSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.SSECustomerAlgorithm));

	if (m_RequestData.SSECustomerKey.IsEmpty() == false)
		Request.SetSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKey));

	if (m_RequestData.SSECustomerKeyMD5.IsEmpty() == false)
		Request.SetSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKeyMD5));

	if (m_RequestData.SSEKMSKeyId.IsEmpty() == false)
		Request.SetSSEKMSKeyId(TCHAR_TO_UTF8(*m_RequestData.SSEKMSKeyId));

	if (m_RequestData.SSEKMSEncryptionContext.IsEmpty() == false)
		Request.SetSSEKMSEncryptionContext(TCHAR_TO_UTF8(*m_RequestData.SSEKMSEncryptionContext));

	if (m_RequestData.CopySourceSSECustomerAlgorithm.IsEmpty() == false)
		Request.SetCopySourceSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.CopySourceSSECustomerAlgorithm));

	if (m_RequestData.CopySourceSSECustomerKey.IsEmpty() == false)
		Request.SetCopySourceSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.CopySourceSSECustomerKey));

	if (m_RequestData.CopySourceSSECustomerKeyMD5.IsEmpty() == false)
		Request.SetCopySourceSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.CopySourceSSECustomerKeyMD5));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.Tagging.IsEmpty() == false)
		Request.SetTagging(TCHAR_TO_UTF8(*m_RequestData.Tagging));

	if (m_RequestData.ObjectLockMode != EObjectLockMode::NOT_SET)
		Request.SetObjectLockMode(static_cast<Aws::S3::Model::ObjectLockMode>(m_RequestData.ObjectLockMode));

	if (m_RequestData.ObjectLockRetainUntilDate > 0)
		Request.SetObjectLockRetainUntilDate(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.ObjectLockRetainUntilDate.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.ObjectLockLegalHoldStatus != EObjectLockLegalHoldStatus::NOT_SET)
		Request.SetObjectLockLegalHoldStatus(static_cast<Aws::S3::Model::ObjectLockLegalHoldStatus>(m_RequestData.ObjectLockLegalHoldStatus));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::CopyObjectRequest&, Aws::S3::Model::CopyObjectOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->CopyObjectAsync(Request, CallbackLambda);
#endif
}

US3CreateBucket* US3CreateBucket::CreateBucket(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FCreateBucketRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3CreateBucket>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3CreateBucket::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::CreateBucketRequest Request = {};

	if (m_RequestData.ACL != EBucketCannedACL::NOT_SET)
		Request.SetACL(static_cast<Aws::S3::Model::BucketCannedACL>(m_RequestData.ACL));

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetCreateBucketConfiguration(m_RequestData.CreateBucketConfiguration);

	if (m_RequestData.GrantFullControl.IsEmpty() == false)
		Request.SetGrantFullControl(TCHAR_TO_UTF8(*m_RequestData.GrantFullControl));

	if (m_RequestData.GrantRead.IsEmpty() == false)
		Request.SetGrantRead(TCHAR_TO_UTF8(*m_RequestData.GrantRead));

	if (m_RequestData.GrantReadACP.IsEmpty() == false)
		Request.SetGrantReadACP(TCHAR_TO_UTF8(*m_RequestData.GrantReadACP));

	if (m_RequestData.GrantWrite.IsEmpty() == false)
		Request.SetGrantWrite(TCHAR_TO_UTF8(*m_RequestData.GrantWrite));

	if (m_RequestData.GrantWriteACP.IsEmpty() == false)
		Request.SetGrantWriteACP(TCHAR_TO_UTF8(*m_RequestData.GrantWriteACP));

	Request.SetObjectLockEnabledForBucket(m_RequestData.bObjectLockEnabledForBucket);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::CreateBucketRequest&, Aws::S3::Model::CreateBucketOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->CreateBucketAsync(Request, CallbackLambda);
#endif
}

US3CreateMultipartUpload* US3CreateMultipartUpload::CreateMultipartUpload(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FCreateMultipartUploadRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3CreateMultipartUpload>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3CreateMultipartUpload::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::CreateMultipartUploadRequest Request = {};

	if (m_RequestData.ACL != EObjectCannedACL::NOT_SET)
		Request.SetACL(static_cast<Aws::S3::Model::ObjectCannedACL>(m_RequestData.ACL));

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));
	if (m_RequestData.CacheControl.IsEmpty() == false)
		Request.SetCacheControl(TCHAR_TO_UTF8(*m_RequestData.CacheControl));
	if (m_RequestData.ContentDisposition.IsEmpty() == false)
		Request.SetContentDisposition(TCHAR_TO_UTF8(*m_RequestData.ContentDisposition));
	if (m_RequestData.ContentEncoding.IsEmpty() == false)
		Request.SetContentEncoding(TCHAR_TO_UTF8(*m_RequestData.ContentEncoding));
	if (m_RequestData.ContentLanguage.IsEmpty() == false)
		Request.SetContentLanguage(TCHAR_TO_UTF8(*m_RequestData.ContentLanguage));
	if (m_RequestData.ContentType.IsEmpty() == false)
		Request.SetContentType(TCHAR_TO_UTF8(*m_RequestData.ContentType));
	if (m_RequestData.Expires > 0)
		Request.SetExpires(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.Expires.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));
	if (m_RequestData.GrantFullControl.IsEmpty() == false)
		Request.SetGrantFullControl(TCHAR_TO_UTF8(*m_RequestData.GrantFullControl));
	if (m_RequestData.GrantRead.IsEmpty() == false)
		Request.SetGrantRead(TCHAR_TO_UTF8(*m_RequestData.GrantRead));
	if (m_RequestData.GrantReadACP.IsEmpty() == false)
		Request.SetGrantReadACP(TCHAR_TO_UTF8(*m_RequestData.GrantReadACP));
	if (m_RequestData.GrantWriteACP.IsEmpty() == false)
		Request.SetGrantWriteACP(TCHAR_TO_UTF8(*m_RequestData.GrantWriteACP));
	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	for (auto& Element : m_RequestData.Metadata)
		Request.AddMetadata(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	if (m_RequestData.ServerSideEncryption != EServerSideEncryption::NOT_SET)
		Request.SetServerSideEncryption(static_cast<Aws::S3::Model::ServerSideEncryption>(m_RequestData.ServerSideEncryption));
	if (m_RequestData.StorageClass != EStorageClass::NOT_SET)
		Request.SetStorageClass(static_cast<Aws::S3::Model::StorageClass>(m_RequestData.StorageClass));

	if (m_RequestData.WebsiteRedirectLocation.IsEmpty() == false)
		Request.SetWebsiteRedirectLocation(TCHAR_TO_UTF8(*m_RequestData.WebsiteRedirectLocation));
	if (m_RequestData.SSECustomerAlgorithm.IsEmpty() == false)
		Request.SetSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.SSECustomerAlgorithm));
	if (m_RequestData.SSECustomerKey.IsEmpty() == false)
		Request.SetSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKey));
	if (m_RequestData.SSECustomerKeyMD5.IsEmpty() == false)
		Request.SetSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKeyMD5));
	if (m_RequestData.SSEKMSKeyId.IsEmpty() == false)
		Request.SetSSEKMSKeyId(TCHAR_TO_UTF8(*m_RequestData.SSEKMSKeyId));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.Tagging.IsEmpty() == false)
		Request.SetTagging(TCHAR_TO_UTF8(*m_RequestData.Tagging));

	if (m_RequestData.ObjectLockMode != EObjectLockMode::NOT_SET)
		Request.SetObjectLockMode(static_cast<Aws::S3::Model::ObjectLockMode>(m_RequestData.ObjectLockMode));

	if (m_RequestData.ObjectLockRetainUntilDate > 0)
		Request.SetObjectLockRetainUntilDate(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.ObjectLockRetainUntilDate.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.ObjectLockLegalHoldStatus != EObjectLockLegalHoldStatus::NOT_SET)
		Request.SetObjectLockLegalHoldStatus(static_cast<Aws::S3::Model::ObjectLockLegalHoldStatus>(m_RequestData.ObjectLockLegalHoldStatus));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));


	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::CreateMultipartUploadRequest&, Aws::S3::Model::CreateMultipartUploadOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->CreateMultipartUploadAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucket* US3DeleteBucket::DeleteBucket(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucket>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucket::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketRequest&, Aws::S3::Model::DeleteBucketOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketAnalyticsConfiguration* US3DeleteBucketAnalyticsConfiguration::DeleteBucketAnalyticsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketAnalyticsConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketAnalyticsConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketAnalyticsConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketAnalyticsConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Id.IsEmpty() == false)
		Request.SetId(TCHAR_TO_UTF8(*m_RequestData.Id));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketAnalyticsConfigurationRequest&, Aws::S3::Model::DeleteBucketAnalyticsConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketAnalyticsConfigurationAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketCors* US3DeleteBucketCors::DeleteBucketCors(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketCorsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketCors>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketCors::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketCorsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketCorsRequest&, Aws::S3::Model::DeleteBucketCorsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketCorsAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketEncryption* US3DeleteBucketEncryption::DeleteBucketEncryption(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketEncryptionRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketEncryption>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketEncryption::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketEncryptionRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketEncryptionRequest&, Aws::S3::Model::DeleteBucketEncryptionOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketEncryptionAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketInventoryConfiguration* US3DeleteBucketInventoryConfiguration::DeleteBucketInventoryConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketInventoryConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketInventoryConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketInventoryConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketInventoryConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Id.IsEmpty() == false)
		Request.SetId(TCHAR_TO_UTF8(*m_RequestData.Id));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketInventoryConfigurationRequest&, Aws::S3::Model::DeleteBucketInventoryConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketInventoryConfigurationAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketLifecycle* US3DeleteBucketLifecycle::DeleteBucketLifecycle(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketLifecycleRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketLifecycle>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketLifecycle::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketLifecycleRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketLifecycleRequest&, Aws::S3::Model::DeleteBucketLifecycleOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketLifecycleAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketMetricsConfiguration* US3DeleteBucketMetricsConfiguration::DeleteBucketMetricsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketMetricsConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketMetricsConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketMetricsConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketMetricsConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketMetricsConfigurationRequest&, Aws::S3::Model::DeleteBucketMetricsConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketMetricsConfigurationAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketPolicy* US3DeleteBucketPolicy::DeleteBucketPolicy(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketPolicyRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketPolicy>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketPolicy::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketPolicyRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketPolicyRequest&, Aws::S3::Model::DeleteBucketPolicyOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketPolicyAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketReplication* US3DeleteBucketReplication::DeleteBucketReplication(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketReplicationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketReplication>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketReplication::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketReplicationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketReplicationRequest&, Aws::S3::Model::DeleteBucketReplicationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketReplicationAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketTagging* US3DeleteBucketTagging::DeleteBucketTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketTaggingRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketTagging>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketTagging::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketTaggingRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketTaggingRequest&, Aws::S3::Model::DeleteBucketTaggingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketTaggingAsync(Request, CallbackLambda);
#endif
}

US3DeleteBucketWebsite* US3DeleteBucketWebsite::DeleteBucketWebsite(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketWebsiteRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteBucketWebsite>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteBucketWebsite::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteBucketWebsiteRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteBucketWebsiteRequest&, Aws::S3::Model::DeleteBucketWebsiteOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteBucketWebsiteAsync(Request, CallbackLambda);
#endif
}

US3DeleteObject* US3DeleteObject::DeleteObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteObjectRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteObject>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteObject::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteObjectRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.MFA.IsEmpty() == false)
		Request.SetMFA(TCHAR_TO_UTF8(*m_RequestData.MFA));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.bBypassGovernanceRetention)
		Request.SetBypassGovernanceRetention(m_RequestData.bBypassGovernanceRetention);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteObjectRequest&, Aws::S3::Model::DeleteObjectOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteObjectAsync(Request, CallbackLambda);
#endif
}

US3DeleteObjectTagging* US3DeleteObjectTagging::DeleteObjectTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteObjectTaggingRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteObjectTagging>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteObjectTagging::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteObjectTaggingRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteObjectTaggingRequest&, Aws::S3::Model::DeleteObjectTaggingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteObjectTaggingAsync(Request, CallbackLambda);
#endif
}

US3DeleteObjects* US3DeleteObjects::DeleteObjects(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteObjectsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeleteObjects>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeleteObjects::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeleteObjectsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetDelete(m_RequestData.Delete);

	if (m_RequestData.MFA.IsEmpty() == false)
		Request.SetMFA(TCHAR_TO_UTF8(*m_RequestData.MFA));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	Request.SetBypassGovernanceRetention(m_RequestData.bBypassGovernanceRetention);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeleteObjectsRequest&, Aws::S3::Model::DeleteObjectsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeleteObjectsAsync(Request, CallbackLambda);
#endif
}

US3DeletePublicAccessBlock* US3DeletePublicAccessBlock::DeletePublicAccessBlock(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeletePublicAccessBlockRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3DeletePublicAccessBlock>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3DeletePublicAccessBlock::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::DeletePublicAccessBlockRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::DeletePublicAccessBlockRequest&, Aws::S3::Model::DeletePublicAccessBlockOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->DeletePublicAccessBlockAsync(Request, CallbackLambda);
#endif
}

US3GetBucketAccelerateConfiguration* US3GetBucketAccelerateConfiguration::GetBucketAccelerateConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketAccelerateConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketAccelerateConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketAccelerateConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketAccelerateConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketAccelerateConfigurationRequest&, Aws::S3::Model::GetBucketAccelerateConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketAccelerateConfigurationAsync(Request, CallbackLambda);
#endif
}

US3GetBucketAcl* US3GetBucketAcl::GetBucketAcl(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketAclRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketAcl>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketAcl::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketAclRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketAclRequest&, Aws::S3::Model::GetBucketAclOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketAclAsync(Request, CallbackLambda);
#endif
}

US3GetBucketAnalyticsConfiguration* US3GetBucketAnalyticsConfiguration::GetBucketAnalyticsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketAnalyticsConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketAnalyticsConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketAnalyticsConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketAnalyticsConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Id.IsEmpty() == false)
		Request.SetId(TCHAR_TO_UTF8(*m_RequestData.Id));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketAnalyticsConfigurationRequest&, Aws::S3::Model::GetBucketAnalyticsConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketAnalyticsConfigurationAsync(Request, CallbackLambda);
#endif
}

US3GetBucketCors* US3GetBucketCors::GetBucketCors(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketCorsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketCors>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketCors::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketCorsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketCorsRequest&, Aws::S3::Model::GetBucketCorsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketCorsAsync(Request, CallbackLambda);
#endif
}

US3GetBucketEncryption* US3GetBucketEncryption::GetBucketEncryption(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketEncryptionRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketEncryption>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketEncryption::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketEncryptionRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketEncryptionRequest&, Aws::S3::Model::GetBucketEncryptionOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketEncryptionAsync(Request, CallbackLambda);
#endif
}

US3GetBucketInventoryConfiguration* US3GetBucketInventoryConfiguration::GetBucketInventoryConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketInventoryConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketInventoryConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketInventoryConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketInventoryConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Id.IsEmpty() == false)
		Request.SetId(TCHAR_TO_UTF8(*m_RequestData.Id));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketInventoryConfigurationRequest&, Aws::S3::Model::GetBucketInventoryConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketInventoryConfigurationAsync(Request, CallbackLambda);
#endif
}

US3GetBucketLifecycleConfiguration* US3GetBucketLifecycleConfiguration::GetBucketLifecycleConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketLifecycleConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketLifecycleConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketLifecycleConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketLifecycleConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketLifecycleConfigurationRequest&, Aws::S3::Model::GetBucketLifecycleConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketLifecycleConfigurationAsync(Request, CallbackLambda);
#endif
}

US3GetBucketLocation* US3GetBucketLocation::GetBucketLocation(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketLocationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketLocation>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketLocation::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketLocationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketLocationRequest&, Aws::S3::Model::GetBucketLocationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketLocationAsync(Request, CallbackLambda);
#endif
}

US3GetBucketLogging* US3GetBucketLogging::GetBucketLogging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketLoggingRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketLogging>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketLogging::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketLoggingRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketLoggingRequest&, Aws::S3::Model::GetBucketLoggingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketLoggingAsync(Request, CallbackLambda);
#endif
}

US3GetBucketMetricsConfiguration* US3GetBucketMetricsConfiguration::GetBucketMetricsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketMetricsConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketMetricsConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketMetricsConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketMetricsConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Id.IsEmpty() == false)
		Request.SetId(TCHAR_TO_UTF8(*m_RequestData.Id));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketMetricsConfigurationRequest&, Aws::S3::Model::GetBucketMetricsConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketMetricsConfigurationAsync(Request, CallbackLambda);
#endif
}

US3GetBucketNotificationConfiguration* US3GetBucketNotificationConfiguration::GetBucketNotificationConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketNotificationConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketNotificationConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketNotificationConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketNotificationConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketNotificationConfigurationRequest&, Aws::S3::Model::GetBucketNotificationConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketNotificationConfigurationAsync(Request, CallbackLambda);
#endif
}

US3GetBucketPolicy* US3GetBucketPolicy::GetBucketPolicy(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketPolicyRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketPolicy>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketPolicy::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketPolicyRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketPolicyRequest&, Aws::S3::Model::GetBucketPolicyOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		bool bIsSuccess = outcome.IsSuccess();

		ES3Errors Error = static_cast<ES3Errors>(outcome.GetError().GetErrorType());
		FString ErrorMessage = UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str());

		FGetBucketPolicyResult Result;

		if (bIsSuccess)
		{
			std::ostringstream string;
			string << outcome.GetResult().GetPolicy().rdbuf();

			Result.Response = string.str().c_str();
		}

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(bIsSuccess, Result, Error, ErrorMessage);
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketPolicyAsync(Request, CallbackLambda);
#endif
}

US3GetBucketPolicyStatus* US3GetBucketPolicyStatus::GetBucketPolicyStatus(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketPolicyStatusRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketPolicyStatus>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketPolicyStatus::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketPolicyStatusRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketPolicyStatusRequest&, Aws::S3::Model::GetBucketPolicyStatusOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketPolicyStatusAsync(Request, CallbackLambda);
#endif
}

US3GetBucketReplication* US3GetBucketReplication::GetBucketReplication(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketReplicationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketReplication>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketReplication::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketReplicationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketReplicationRequest&, Aws::S3::Model::GetBucketReplicationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketReplicationAsync(Request, CallbackLambda);
#endif
}

US3GetBucketRequestPayment* US3GetBucketRequestPayment::GetBucketRequestPayment(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketRequestPaymentRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketRequestPayment>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketRequestPayment::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketRequestPaymentRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketRequestPaymentRequest&, Aws::S3::Model::GetBucketRequestPaymentOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketRequestPaymentAsync(Request, CallbackLambda);
#endif
}

US3GetBucketTagging* US3GetBucketTagging::GetBucketTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketTaggingRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketTagging>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketTagging::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketTaggingRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketTaggingRequest&, Aws::S3::Model::GetBucketTaggingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketTaggingAsync(Request, CallbackLambda);
#endif
}

US3GetBucketVersioning* US3GetBucketVersioning::GetBucketVersioning(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketVersioningRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketVersioning>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketVersioning::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketVersioningRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketVersioningRequest&, Aws::S3::Model::GetBucketVersioningOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketVersioningAsync(Request, CallbackLambda);
#endif
}

US3GetBucketWebsite* US3GetBucketWebsite::GetBucketWebsite(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketWebsiteRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetBucketWebsite>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetBucketWebsite::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetBucketWebsiteRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetBucketWebsiteRequest&, Aws::S3::Model::GetBucketWebsiteOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetBucketWebsiteAsync(Request, CallbackLambda);
#endif
}

US3GetObjectAcl* US3GetObjectAcl::GetObjectAcl(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectAclRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetObjectAcl>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetObjectAcl::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetObjectAclRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetObjectAclRequest&, Aws::S3::Model::GetObjectAclOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetObjectAclAsync(Request, CallbackLambda);
#endif
}

US3GetObjectLegalHold* US3GetObjectLegalHold::GetObjectLegalHold(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectLegalHoldRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetObjectLegalHold>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetObjectLegalHold::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetObjectLegalHoldRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetObjectLegalHoldRequest&, Aws::S3::Model::GetObjectLegalHoldOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetObjectLegalHoldAsync(Request, CallbackLambda);
#endif
}

US3GetObjectLockConfiguration* US3GetObjectLockConfiguration::GetObjectLockConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectLockConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetObjectLockConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetObjectLockConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetObjectLockConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetObjectLockConfigurationRequest&, Aws::S3::Model::GetObjectLockConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetObjectLockConfigurationAsync(Request, CallbackLambda);
#endif
}

US3GetObjectRetention* US3GetObjectRetention::GetObjectRetention(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectRetentionRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetObjectRetention>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetObjectRetention::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetObjectRetentionRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetObjectRetentionRequest&, Aws::S3::Model::GetObjectRetentionOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetObjectRetentionAsync(Request, CallbackLambda);
#endif
}

US3GetObjectTagging* US3GetObjectTagging::GetObjectTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectTaggingRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetObjectTagging>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetObjectTagging::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetObjectTaggingRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetObjectTaggingRequest&, Aws::S3::Model::GetObjectTaggingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetObjectTaggingAsync(Request, CallbackLambda);
#endif
}

US3GetPublicAccessBlock* US3GetPublicAccessBlock::GetPublicAccessBlock(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetPublicAccessBlockRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3GetPublicAccessBlock>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3GetPublicAccessBlock::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::GetPublicAccessBlockRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::GetPublicAccessBlockRequest&, Aws::S3::Model::GetPublicAccessBlockOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->GetPublicAccessBlockAsync(Request, CallbackLambda);
#endif
}

US3HeadBucket* US3HeadBucket::HeadBucket(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FHeadBucketRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3HeadBucket>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3HeadBucket::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::HeadBucketRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::HeadBucketRequest&, Aws::S3::Model::HeadBucketOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->HeadBucketAsync(Request, CallbackLambda);
#endif
}

US3HeadObject* US3HeadObject::HeadObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FHeadObjectRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3HeadObject>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3HeadObject::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::HeadObjectRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.IfMatch.IsEmpty() == false)
		Request.SetIfMatch(TCHAR_TO_UTF8(*m_RequestData.IfMatch));

	if (m_RequestData.IfModifiedSince > 0)
		Request.SetIfModifiedSince(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.IfModifiedSince.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.IfNoneMatch.IsEmpty() == false)
		Request.SetIfNoneMatch(TCHAR_TO_UTF8(*m_RequestData.IfNoneMatch));

	if (m_RequestData.IfUnmodifiedSince > 0)
		Request.SetIfUnmodifiedSince(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.IfUnmodifiedSince.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.Range.IsEmpty() == false)
		Request.SetRange(TCHAR_TO_UTF8(*m_RequestData.Range));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	if (m_RequestData.SSECustomerAlgorithm.IsEmpty() == false)
		Request.SetSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.SSECustomerAlgorithm));

	if (m_RequestData.SSECustomerKey.IsEmpty() == false)
		Request.SetSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKey));

	if (m_RequestData.SSECustomerKeyMD5.IsEmpty() == false)
		Request.SetSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKeyMD5));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.PartNumber > 0)
		Request.SetPartNumber(m_RequestData.PartNumber);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::HeadObjectRequest&, Aws::S3::Model::HeadObjectOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->HeadObjectAsync(Request, CallbackLambda);
#endif
}

US3ListBucketAnalyticsConfigurations* US3ListBucketAnalyticsConfigurations::ListBucketAnalyticsConfigurations(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListBucketAnalyticsConfigurationsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListBucketAnalyticsConfigurations>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3ListBucketAnalyticsConfigurations::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::ListBucketAnalyticsConfigurationsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.ContinuationToken.IsEmpty() == false)
		Request.SetContinuationToken(TCHAR_TO_UTF8(*m_RequestData.ContinuationToken));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::ListBucketAnalyticsConfigurationsRequest&, Aws::S3::Model::ListBucketAnalyticsConfigurationsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListBucketAnalyticsConfigurationsAsync(Request, CallbackLambda);
#endif
}

US3ListBucketInventoryConfigurations* US3ListBucketInventoryConfigurations::ListBucketInventoryConfigurations(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListBucketInventoryConfigurationsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListBucketInventoryConfigurations>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3ListBucketInventoryConfigurations::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::ListBucketInventoryConfigurationsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.ContinuationToken.IsEmpty() == false)
		Request.SetContinuationToken(TCHAR_TO_UTF8(*m_RequestData.ContinuationToken));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::ListBucketInventoryConfigurationsRequest&, Aws::S3::Model::ListBucketInventoryConfigurationsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListBucketInventoryConfigurationsAsync(Request, CallbackLambda);
#endif
}

US3ListBucketMetricsConfigurations* US3ListBucketMetricsConfigurations::ListBucketMetricsConfigurations(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListBucketMetricsConfigurationsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListBucketMetricsConfigurations>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3ListBucketMetricsConfigurations::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::ListBucketMetricsConfigurationsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.ContinuationToken.IsEmpty() == false)
		Request.SetContinuationToken(TCHAR_TO_UTF8(*m_RequestData.ContinuationToken));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::ListBucketMetricsConfigurationsRequest&, Aws::S3::Model::ListBucketMetricsConfigurationsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListBucketMetricsConfigurationsAsync(Request, CallbackLambda);
#endif
}

US3ListBuckets* US3ListBuckets::ListBuckets(UObject* WorldContextObject, UAwsS3ClientObject* S3object)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListBuckets>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;

	return Platform;
}

void US3ListBuckets::Activate()
{
	LogVerbose("");

#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	auto CallbackLambda = [this](const Aws::S3::S3Client*, Aws::S3::Model::ListBucketsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListBucketsAsync(CallbackLambda);
#endif
}

US3ListMultipartUploads* US3ListMultipartUploads::ListMultipartUploads(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListMultipartUploadsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListMultipartUploads>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3ListMultipartUploads::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::ListMultipartUploadsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Delimiter.IsEmpty() == false)
		Request.SetDelimiter(TCHAR_TO_UTF8(*m_RequestData.Delimiter));

	if (m_RequestData.EncodingType != EEncodingType::NOT_SET)
		Request.SetEncodingType(static_cast<Aws::S3::Model::EncodingType>(m_RequestData.EncodingType));

	if (m_RequestData.KeyMarker.IsEmpty() == false)
		Request.SetKeyMarker(TCHAR_TO_UTF8(*m_RequestData.KeyMarker));

	if (m_RequestData.MaxUploads > 0)
		Request.SetMaxUploads(m_RequestData.MaxUploads);

	if (m_RequestData.Prefix.IsEmpty() == false)
		Request.SetPrefix(TCHAR_TO_UTF8(*m_RequestData.Prefix));

	if (m_RequestData.UploadIdMarker.IsEmpty() == false)
		Request.SetUploadIdMarker(TCHAR_TO_UTF8(*m_RequestData.UploadIdMarker));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::ListMultipartUploadsRequest&, Aws::S3::Model::ListMultipartUploadsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListMultipartUploadsAsync(Request, CallbackLambda);
#endif
}

US3ListObjectVersions* US3ListObjectVersions::ListObjectVersions(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListObjectVersionsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListObjectVersions>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3ListObjectVersions::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::ListObjectVersionsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Delimiter.IsEmpty() == false)
		Request.SetDelimiter(TCHAR_TO_UTF8(*m_RequestData.Delimiter));

	if (m_RequestData.EncodingType != EEncodingType::NOT_SET)
		Request.SetEncodingType(static_cast<Aws::S3::Model::EncodingType>(m_RequestData.EncodingType));

	if (m_RequestData.KeyMarker.IsEmpty() == false)
		Request.SetKeyMarker(TCHAR_TO_UTF8(*m_RequestData.KeyMarker));

	if (m_RequestData.MaxKeys > 0)
		Request.SetMaxKeys(m_RequestData.MaxKeys);

	if (m_RequestData.Prefix.IsEmpty() == false)
		Request.SetPrefix(TCHAR_TO_UTF8(*m_RequestData.Prefix));

	if (m_RequestData.VersionIdMarker.IsEmpty() == false)
		Request.SetVersionIdMarker(TCHAR_TO_UTF8(*m_RequestData.VersionIdMarker));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::ListObjectVersionsRequest&, Aws::S3::Model::ListObjectVersionsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListObjectVersionsAsync(Request, CallbackLambda);
#endif
}

US3ListObjects* US3ListObjects::ListObjects(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListObjectsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListObjects>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3ListObjects::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::ListObjectsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Delimiter.IsEmpty() == false)
		Request.SetDelimiter(TCHAR_TO_UTF8(*m_RequestData.Delimiter));

	if (m_RequestData.EncodingType != EEncodingType::NOT_SET)
		Request.SetEncodingType(static_cast<Aws::S3::Model::EncodingType>(m_RequestData.EncodingType));

	if (m_RequestData.Marker.IsEmpty() == false)
		Request.SetMarker(TCHAR_TO_UTF8(*m_RequestData.Marker));

	if (m_RequestData.MaxKeys > 0)
		Request.SetMaxKeys(m_RequestData.MaxKeys);

	if (m_RequestData.Prefix.IsEmpty() == false)
		Request.SetPrefix(TCHAR_TO_UTF8(*m_RequestData.Prefix));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::ListObjectsRequest&, Aws::S3::Model::ListObjectsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListObjectsAsync(Request, CallbackLambda);
#endif
}

US3ListObjectsV2* US3ListObjectsV2::ListObjectsV2(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListObjectsV2Request& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListObjectsV2>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3ListObjectsV2::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::ListObjectsV2Request Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Delimiter.IsEmpty() == false)
		Request.SetDelimiter(TCHAR_TO_UTF8(*m_RequestData.Delimiter));

	if (m_RequestData.EncodingType != EEncodingType::NOT_SET)
		Request.SetEncodingType(static_cast<Aws::S3::Model::EncodingType>(m_RequestData.EncodingType));

	if (m_RequestData.MaxKeys > 0)
		Request.SetMaxKeys(m_RequestData.MaxKeys);

	if (m_RequestData.Prefix.IsEmpty() == false)
		Request.SetPrefix(TCHAR_TO_UTF8(*m_RequestData.Prefix));

	if (m_RequestData.ContinuationToken.IsEmpty() == false)
		Request.SetContinuationToken(TCHAR_TO_UTF8(*m_RequestData.ContinuationToken));

	Request.SetFetchOwner(m_RequestData.bFetchOwner);

	if (m_RequestData.StartAfter.IsEmpty() == false)
		Request.SetStartAfter(TCHAR_TO_UTF8(*m_RequestData.StartAfter));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::ListObjectsV2Request&, Aws::S3::Model::ListObjectsV2Outcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListObjectsV2Async(Request, CallbackLambda);
#endif
}

US3ListParts* US3ListParts::ListParts(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListPartsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3ListParts>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3ListParts::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::ListPartsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.MaxParts > 0)
		Request.SetMaxParts(m_RequestData.MaxParts);

	if (m_RequestData.PartNumberMarker > 0)
		Request.SetPartNumberMarker(m_RequestData.PartNumberMarker);

	if (m_RequestData.UploadId.IsEmpty() == false)
		Request.SetUploadId(TCHAR_TO_UTF8(*m_RequestData.UploadId));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::ListPartsRequest&, Aws::S3::Model::ListPartsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->ListPartsAsync(Request, CallbackLambda);
#endif
}

US3PutBucketAccelerateConfiguration* US3PutBucketAccelerateConfiguration::PutBucketAccelerateConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketAccelerateConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketAccelerateConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketAccelerateConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketAccelerateConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetAccelerateConfiguration(m_RequestData.AccelerateConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketAccelerateConfigurationRequest&, Aws::S3::Model::PutBucketAccelerateConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketAccelerateConfigurationAsync(Request, CallbackLambda);
#endif
}

US3PutBucketAcl* US3PutBucketAcl::PutBucketAcl(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketAclRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketAcl>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketAcl::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketAclRequest Request = {};

	if (m_RequestData.ACL != EBucketCannedACL::NOT_SET)
		Request.SetACL(static_cast<Aws::S3::Model::BucketCannedACL>(m_RequestData.ACL));

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetAccessControlPolicy(m_RequestData.AccessControlPolicy);

	if (m_RequestData.GrantFullControl.IsEmpty() == false)
		Request.SetGrantFullControl(TCHAR_TO_UTF8(*m_RequestData.GrantFullControl));

	if (m_RequestData.GrantRead.IsEmpty() == false)
		Request.SetGrantRead(TCHAR_TO_UTF8(*m_RequestData.GrantRead));

	if (m_RequestData.GrantReadACP.IsEmpty() == false)
		Request.SetGrantReadACP(TCHAR_TO_UTF8(*m_RequestData.GrantReadACP));

	if (m_RequestData.GrantWrite.IsEmpty() == false)
		Request.SetGrantWrite(TCHAR_TO_UTF8(*m_RequestData.GrantWrite));

	if (m_RequestData.GrantWriteACP.IsEmpty() == false)
		Request.SetGrantWriteACP(TCHAR_TO_UTF8(*m_RequestData.GrantWriteACP));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketAclRequest&, Aws::S3::Model::PutBucketAclOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketAclAsync(Request, CallbackLambda);
#endif
}

US3PutBucketAnalyticsConfiguration* US3PutBucketAnalyticsConfiguration::PutBucketAnalyticsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketAnalyticsConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketAnalyticsConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketAnalyticsConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketAnalyticsConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Id.IsEmpty() == false)
		Request.SetId(TCHAR_TO_UTF8(*m_RequestData.Id));

	Request.SetAnalyticsConfiguration(m_RequestData.AnalyticsConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketAnalyticsConfigurationRequest&, Aws::S3::Model::PutBucketAnalyticsConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketAnalyticsConfigurationAsync(Request, CallbackLambda);
#endif
}

US3PutBucketCors* US3PutBucketCors::PutBucketCors(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketCorsRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketCors>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketCors::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketCorsRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetCORSConfiguration(m_RequestData.CORSConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketCorsRequest&, Aws::S3::Model::PutBucketCorsOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketCorsAsync(Request, CallbackLambda);
#endif
}

US3PutBucketEncryption* US3PutBucketEncryption::PutBucketEncryption(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketEncryptionRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketEncryption>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketEncryption::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketEncryptionRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetServerSideEncryptionConfiguration(m_RequestData.ServerSideEncryptionConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketEncryptionRequest&, Aws::S3::Model::PutBucketEncryptionOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketEncryptionAsync(Request, CallbackLambda);
#endif
}

US3PutBucketInventoryConfiguration* US3PutBucketInventoryConfiguration::PutBucketInventoryConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketInventoryConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketInventoryConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketInventoryConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketInventoryConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Id.IsEmpty() == false)
		Request.SetId(TCHAR_TO_UTF8(*m_RequestData.Id));

	Request.SetInventoryConfiguration(m_RequestData.InventoryConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketInventoryConfigurationRequest&, Aws::S3::Model::PutBucketInventoryConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketInventoryConfigurationAsync(Request, CallbackLambda);
#endif
}

US3PutBucketLifecycleConfiguration* US3PutBucketLifecycleConfiguration::PutBucketLifecycleConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketLifecycleConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketLifecycleConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketLifecycleConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketLifecycleConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetLifecycleConfiguration(m_RequestData.LifecycleConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketLifecycleConfigurationRequest&, Aws::S3::Model::PutBucketLifecycleConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketLifecycleConfigurationAsync(Request, CallbackLambda);
#endif
}

US3PutBucketLogging* US3PutBucketLogging::PutBucketLogging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketLoggingRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketLogging>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketLogging::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketLoggingRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetBucketLoggingStatus(m_RequestData.BucketLoggingStatus);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketLoggingRequest&, Aws::S3::Model::PutBucketLoggingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketLoggingAsync(Request, CallbackLambda);
#endif
}

US3PutBucketMetricsConfiguration* US3PutBucketMetricsConfiguration::PutBucketMetricsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketMetricsConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketMetricsConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketMetricsConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketMetricsConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Id.IsEmpty() == false)
		Request.SetId(TCHAR_TO_UTF8(*m_RequestData.Id));

	Request.SetMetricsConfiguration(m_RequestData.MetricsConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketMetricsConfigurationRequest&, Aws::S3::Model::PutBucketMetricsConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketMetricsConfigurationAsync(Request, CallbackLambda);
#endif
}

US3PutBucketNotificationConfiguration* US3PutBucketNotificationConfiguration::PutBucketNotificationConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketNotificationConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketNotificationConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketNotificationConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketNotificationConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetNotificationConfiguration(m_RequestData.NotificationConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketNotificationConfigurationRequest&, Aws::S3::Model::PutBucketNotificationConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketNotificationConfigurationAsync(Request, CallbackLambda);
#endif
}

US3PutBucketPolicy* US3PutBucketPolicy::PutBucketPolicy(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketPolicyRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketPolicy>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketPolicy::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketPolicyRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetConfirmRemoveSelfBucketAccess(m_RequestData.bConfirmRemoveSelfBucketAccess);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketPolicyRequest&, Aws::S3::Model::PutBucketPolicyOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketPolicyAsync(Request, CallbackLambda);
#endif
}

US3PutBucketReplication* US3PutBucketReplication::PutBucketReplication(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketReplicationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketReplication>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketReplication::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketReplicationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetReplicationConfiguration(m_RequestData.ReplicationConfiguration);

	if (m_RequestData.Token.IsEmpty() == false)
		Request.SetToken(TCHAR_TO_UTF8(*m_RequestData.Token));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketReplicationRequest&, Aws::S3::Model::PutBucketReplicationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketReplicationAsync(Request, CallbackLambda);
#endif
}

US3PutBucketRequestPayment* US3PutBucketRequestPayment::PutBucketRequestPayment(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketRequestPaymentRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketRequestPayment>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketRequestPayment::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketRequestPaymentRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetRequestPaymentConfiguration(m_RequestData.RequestPaymentConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketRequestPaymentRequest&, Aws::S3::Model::PutBucketRequestPaymentOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketRequestPaymentAsync(Request, CallbackLambda);
#endif
}

US3PutBucketTagging* US3PutBucketTagging::PutBucketTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketTaggingRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketTagging>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketTagging::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketTaggingRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetTagging(m_RequestData.Tagging);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketTaggingRequest&, Aws::S3::Model::PutBucketTaggingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketTaggingAsync(Request, CallbackLambda);
#endif
}

US3PutBucketVersioning* US3PutBucketVersioning::PutBucketVersioning(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketVersioningRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketVersioning>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketVersioning::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketVersioningRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.MFA.IsEmpty() == false)
		Request.SetMFA(TCHAR_TO_UTF8(*m_RequestData.MFA));

	Request.SetVersioningConfiguration(m_RequestData.VersioningConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketVersioningRequest&, Aws::S3::Model::PutBucketVersioningOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketVersioningAsync(Request, CallbackLambda);
#endif
}

US3PutBucketWebsite* US3PutBucketWebsite::PutBucketWebsite(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketWebsiteRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutBucketWebsite>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutBucketWebsite::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutBucketWebsiteRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetWebsiteConfiguration(m_RequestData.WebsiteConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutBucketWebsiteRequest&, Aws::S3::Model::PutBucketWebsiteOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutBucketWebsiteAsync(Request, CallbackLambda);
#endif
}

US3PutObject* US3PutObject::PutObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutObject>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutObject::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* File = PlatformFile.OpenRead(*m_RequestData.File);

	if (File)
	{
		Aws::S3::Model::PutObjectRequest Request = {};

		if (m_RequestData.ACL != EObjectCannedACL::NOT_SET)
			Request.SetACL(static_cast<Aws::S3::Model::ObjectCannedACL>(m_RequestData.ACL));
		if (m_RequestData.Bucket.IsEmpty() == false)
			Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));
		if (m_RequestData.CacheControl.IsEmpty() == false)
			Request.SetCacheControl(TCHAR_TO_UTF8(*m_RequestData.CacheControl));
		if (m_RequestData.ContentDisposition.IsEmpty() == false)
			Request.SetContentDisposition(TCHAR_TO_UTF8(*m_RequestData.ContentDisposition));
		if (m_RequestData.ContentEncoding.IsEmpty() == false)
			Request.SetContentEncoding(TCHAR_TO_UTF8(*m_RequestData.ContentEncoding));
		if (m_RequestData.ContentLanguage.IsEmpty() == false)
			Request.SetContentLanguage(TCHAR_TO_UTF8(*m_RequestData.ContentLanguage));
		if (m_RequestData.ContentLength.IsEmpty() == false)
			Request.SetContentLength(FCString::Atod(*m_RequestData.ContentLength));
		if (m_RequestData.ContentMD5.IsEmpty() == false)
			Request.SetContentMD5(TCHAR_TO_UTF8(*m_RequestData.ContentMD5));
		if (m_RequestData.Expires > 0)
			Request.SetExpires(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.Expires.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));
		if (m_RequestData.GrantFullControl.IsEmpty() == false)
			Request.SetGrantFullControl(TCHAR_TO_UTF8(*m_RequestData.GrantFullControl));
		if (m_RequestData.GrantRead.IsEmpty() == false)
			Request.SetGrantRead(TCHAR_TO_UTF8(*m_RequestData.GrantRead));
		if (m_RequestData.GrantReadACP.IsEmpty() == false)
			Request.SetGrantReadACP(TCHAR_TO_UTF8(*m_RequestData.GrantReadACP));
		if (m_RequestData.GrantWriteACP.IsEmpty() == false)
			Request.SetGrantWriteACP(TCHAR_TO_UTF8(*m_RequestData.GrantWriteACP));
		if (m_RequestData.Key.IsEmpty() == false)
			Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

		for (auto& Element : m_RequestData.Metadata)
			Request.AddMetadata(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

		if (m_RequestData.ServerSideEncryption != EServerSideEncryption::NOT_SET)
			Request.SetServerSideEncryption(static_cast<Aws::S3::Model::ServerSideEncryption>(m_RequestData.ServerSideEncryption));
		if (m_RequestData.StorageClass != EStorageClass::NOT_SET)
			Request.SetStorageClass(static_cast<Aws::S3::Model::StorageClass>(m_RequestData.StorageClass));

		if (m_RequestData.WebsiteRedirectLocation.IsEmpty() == false)
			Request.SetWebsiteRedirectLocation(TCHAR_TO_UTF8(*m_RequestData.WebsiteRedirectLocation));
		if (m_RequestData.SSECustomerAlgorithm.IsEmpty() == false)
			Request.SetSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.SSECustomerAlgorithm));
		if (m_RequestData.SSECustomerKey.IsEmpty() == false)
			Request.SetSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKey));
		if (m_RequestData.SSECustomerKeyMD5.IsEmpty() == false)
			Request.SetSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKeyMD5));
		if (m_RequestData.SSEKMSKeyId.IsEmpty() == false)
			Request.SetSSEKMSKeyId(TCHAR_TO_UTF8(*m_RequestData.SSEKMSKeyId));

		if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
			Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

		if (m_RequestData.Tagging.IsEmpty() == false)
			Request.SetTagging(TCHAR_TO_UTF8(*m_RequestData.Tagging));

		if (m_RequestData.ObjectLockMode != EObjectLockMode::NOT_SET)
			Request.SetObjectLockMode(static_cast<Aws::S3::Model::ObjectLockMode>(m_RequestData.ObjectLockMode));

		if (m_RequestData.ObjectLockRetainUntilDate > 0)
			Request.SetObjectLockRetainUntilDate(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.ObjectLockRetainUntilDate.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

		if (m_RequestData.ObjectLockLegalHoldStatus != EObjectLockLegalHoldStatus::NOT_SET)
			Request.SetObjectLockLegalHoldStatus(static_cast<Aws::S3::Model::ObjectLockLegalHoldStatus>(m_RequestData.ObjectLockLegalHoldStatus));

		for (auto& Element : m_RequestData.CustomizedAccessLogTag)
			Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

		TArray<uint8> FileData;
		FileData.SetNum(File->Size());
		File->Read(FileData.GetData(), File->Size());

		auto data = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream", std::stringstream::in | std::stringstream::out | std::stringstream::binary);
		data->write((char*)FileData.GetData(), File->Size());

		Request.SetBody(data);

		delete File;

		auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutObjectRequest&, Aws::S3::Model::PutObjectOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
		{
			LogVerbose("");

			FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
			{
				Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
				SetReadyToDestroy();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		};

		m_S3Object->GetAwsS3Client()->PutObjectAsync(Request, CallbackLambda);
	}
	else
	{
		LogError("File %s not found", *m_RequestData.File);
	}
#endif
}

US3PutObjectAcl* US3PutObjectAcl::PutObjectAcl(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectAclRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutObjectAcl>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutObjectAcl::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutObjectAclRequest Request = {};

	if (m_RequestData.ACL != EObjectCannedACL::NOT_SET)
		Request.SetACL(static_cast<Aws::S3::Model::ObjectCannedACL>(m_RequestData.ACL));

	Request.SetAccessControlPolicy(m_RequestData.AccessControlPolicy);

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.GrantFullControl.IsEmpty() == false)
		Request.SetGrantFullControl(TCHAR_TO_UTF8(*m_RequestData.GrantFullControl));

	if (m_RequestData.GrantRead.IsEmpty() == false)
		Request.SetGrantRead(TCHAR_TO_UTF8(*m_RequestData.GrantRead));

	if (m_RequestData.GrantReadACP.IsEmpty() == false)
		Request.SetGrantReadACP(TCHAR_TO_UTF8(*m_RequestData.GrantReadACP));

	if (m_RequestData.GrantWrite.IsEmpty() == false)
		Request.SetGrantWrite(TCHAR_TO_UTF8(*m_RequestData.GrantWrite));

	if (m_RequestData.GrantWriteACP.IsEmpty() == false)
		Request.SetGrantWriteACP(TCHAR_TO_UTF8(*m_RequestData.GrantWriteACP));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutObjectAclRequest&, Aws::S3::Model::PutObjectAclOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutObjectAclAsync(Request, CallbackLambda);
#endif
}

US3PutObjectLegalHold* US3PutObjectLegalHold::PutObjectLegalHold(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectLegalHoldRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutObjectLegalHold>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutObjectLegalHold::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutObjectLegalHoldRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	Request.SetLegalHold(m_RequestData.LegalHold);

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutObjectLegalHoldRequest&, Aws::S3::Model::PutObjectLegalHoldOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutObjectLegalHoldAsync(Request, CallbackLambda);
#endif
}

US3PutObjectLockConfiguration* US3PutObjectLockConfiguration::PutObjectLockConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectLockConfigurationRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutObjectLockConfiguration>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutObjectLockConfiguration::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutObjectLockConfigurationRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetObjectLockConfiguration(m_RequestData.ObjectLockConfiguration);

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.Token.IsEmpty() == false)
		Request.SetToken(TCHAR_TO_UTF8(*m_RequestData.Token));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutObjectLockConfigurationRequest&, Aws::S3::Model::PutObjectLockConfigurationOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutObjectLockConfigurationAsync(Request, CallbackLambda);
#endif
}

US3PutObjectRetention* US3PutObjectRetention::PutObjectRetention(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectRetentionRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutObjectRetention>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutObjectRetention::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutObjectRetentionRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	Request.SetRetention(m_RequestData.Retention);

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	Request.SetBypassGovernanceRetention(m_RequestData.bBypassGovernanceRetention);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutObjectRetentionRequest&, Aws::S3::Model::PutObjectRetentionOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutObjectRetentionAsync(Request, CallbackLambda);
#endif
}

US3PutObjectTagging* US3PutObjectTagging::PutObjectTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectTaggingRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutObjectTagging>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutObjectTagging::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutObjectTaggingRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	Request.SetTagging(m_RequestData.Tagging);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutObjectTaggingRequest&, Aws::S3::Model::PutObjectTaggingOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutObjectTaggingAsync(Request, CallbackLambda);
#endif
}

US3PutPublicAccessBlock* US3PutPublicAccessBlock::PutPublicAccessBlock(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutPublicAccessBlockRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3PutPublicAccessBlock>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3PutPublicAccessBlock::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::PutPublicAccessBlockRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	Request.SetPublicAccessBlockConfiguration(m_RequestData.PublicAccessBlockConfiguration);

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::PutPublicAccessBlockRequest&, Aws::S3::Model::PutPublicAccessBlockOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->PutPublicAccessBlockAsync(Request, CallbackLambda);
#endif
}

US3RestoreObject* US3RestoreObject::RestoreObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FRestoreObjectRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3RestoreObject>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3RestoreObject::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::RestoreObjectRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	Request.SetRestoreRequest(m_RequestData.RestoreRequest);

	if (m_RequestData.VersionId.IsEmpty() == false)
		Request.SetVersionId(TCHAR_TO_UTF8(*m_RequestData.VersionId));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::RestoreObjectRequest&, Aws::S3::Model::RestoreObjectOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->RestoreObjectAsync(Request, CallbackLambda);
#endif
}

US3UploadPart* US3UploadPart::UploadPart(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FUploadPartRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3UploadPart>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3UploadPart::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());


	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* File = PlatformFile.OpenRead(*m_RequestData.File);

	if (File)
	{
		Aws::S3::Model::UploadPartRequest Request = {};

		if (m_RequestData.Bucket.IsEmpty() == false)
			Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

		if (m_RequestData.ContentLength.IsEmpty() == false)
			Request.SetContentLength(FCString::Atod(*m_RequestData.ContentLength));

		if (m_RequestData.ContentMD5.IsEmpty() == false)
			Request.SetContentMD5(TCHAR_TO_UTF8(*m_RequestData.ContentMD5));

		if (m_RequestData.Key.IsEmpty() == false)
			Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

		Request.SetPartNumber(m_RequestData.PartNumber);

		if (m_RequestData.UploadId.IsEmpty() == false)
			Request.SetUploadId(TCHAR_TO_UTF8(*m_RequestData.UploadId));

		if (m_RequestData.SSECustomerAlgorithm.IsEmpty() == false)
			Request.SetSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.SSECustomerAlgorithm));

		if (m_RequestData.SSECustomerKey.IsEmpty() == false)
			Request.SetSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKey));

		if (m_RequestData.SSECustomerKeyMD5.IsEmpty() == false)
			Request.SetSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKeyMD5));

		if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
			Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

		for (auto& Element : m_RequestData.CustomizedAccessLogTag)
			Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

		TArray<uint8> FileData;
		FileData.SetNum(File->Size());
		File->Read(FileData.GetData(), File->Size());

		auto data = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream", std::stringstream::in | std::stringstream::out | std::stringstream::binary);
		data->write((char*)FileData.GetData(), File->Size());

		Request.SetBody(data);

		delete File;

		auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::UploadPartRequest&, Aws::S3::Model::UploadPartOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
		{
			LogVerbose("");

			FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
			{
				Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
				SetReadyToDestroy();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		};

		m_S3Object->GetAwsS3Client()->UploadPartAsync(Request, CallbackLambda);
	}
	else
	{
		LogError("File %s not found", *m_RequestData.File);
	}
#endif
}

US3UploadPartCopy* US3UploadPartCopy::UploadPartCopy(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FUploadPartCopyRequest& Request)
{
	LogVerbose("");

	auto Platform = NewObject<US3UploadPartCopy>();
	Platform->AddToRoot();
	Platform->m_S3Object = S3object;
	Platform->m_RequestData = Request;

	return Platform;
}

void US3UploadPartCopy::Activate()
{
#if !DISABLE_S3
	LogVerbose("");
	check(m_S3Object);
	check(m_S3Object->GetAwsS3Client());

	Aws::S3::Model::UploadPartCopyRequest Request = {};

	if (m_RequestData.Bucket.IsEmpty() == false)
		Request.SetBucket(TCHAR_TO_UTF8(*m_RequestData.Bucket));

	if (m_RequestData.CopySource.IsEmpty() == false)
		Request.SetCopySource(TCHAR_TO_UTF8(*m_RequestData.CopySource));

	if (m_RequestData.CopySourceIfMatch.IsEmpty() == false)
		Request.SetCopySourceIfMatch(TCHAR_TO_UTF8(*m_RequestData.CopySourceIfMatch));

	if (m_RequestData.CopySourceIfModifiedSince > 0)
		Request.SetCopySourceIfModifiedSince(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.CopySourceIfModifiedSince.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.CopySourceIfNoneMatch.IsEmpty() == false)
		Request.SetCopySourceIfNoneMatch(TCHAR_TO_UTF8(*m_RequestData.CopySourceIfNoneMatch));

	if (m_RequestData.CopySourceIfUnmodifiedSince > 0)
		Request.SetCopySourceIfUnmodifiedSince(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_RequestData.CopySourceIfUnmodifiedSince.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));

	if (m_RequestData.CopySourceRange.IsEmpty() == false)
		Request.SetCopySourceRange(TCHAR_TO_UTF8(*m_RequestData.CopySourceRange));

	if (m_RequestData.Key.IsEmpty() == false)
		Request.SetKey(TCHAR_TO_UTF8(*m_RequestData.Key));

	Request.SetPartNumber(m_RequestData.PartNumber);

	if (m_RequestData.UploadId.IsEmpty() == false)
		Request.SetUploadId(TCHAR_TO_UTF8(*m_RequestData.UploadId));

	if (m_RequestData.SSECustomerAlgorithm.IsEmpty() == false)
		Request.SetSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.SSECustomerAlgorithm));

	if (m_RequestData.SSECustomerKey.IsEmpty() == false)
		Request.SetSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKey));

	if (m_RequestData.SSECustomerKeyMD5.IsEmpty() == false)
		Request.SetSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.SSECustomerKeyMD5));

	if (m_RequestData.CopySourceSSECustomerAlgorithm.IsEmpty() == false)
		Request.SetCopySourceSSECustomerAlgorithm(TCHAR_TO_UTF8(*m_RequestData.CopySourceSSECustomerAlgorithm));

	if (m_RequestData.CopySourceSSECustomerKey.IsEmpty() == false)
		Request.SetCopySourceSSECustomerKey(TCHAR_TO_UTF8(*m_RequestData.CopySourceSSECustomerKey));

	if (m_RequestData.CopySourceSSECustomerKeyMD5.IsEmpty() == false)
		Request.SetCopySourceSSECustomerKeyMD5(TCHAR_TO_UTF8(*m_RequestData.CopySourceSSECustomerKeyMD5));

	if (m_RequestData.RequestPayer != ERequestPayer::NOT_SET)
		Request.SetRequestPayer(static_cast<Aws::S3::Model::RequestPayer>(m_RequestData.RequestPayer));

	for (auto& Element : m_RequestData.CustomizedAccessLogTag)
		Request.AddCustomizedAccessLogTag(TCHAR_TO_UTF8(*Element.Key), TCHAR_TO_UTF8(*Element.Value));

	auto CallbackLambda = [this](const Aws::S3::S3Client*, const Aws::S3::Model::UploadPartCopyRequest&, Aws::S3::Model::UploadPartCopyOutcome outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>&)
	{
		LogVerbose("");

		FFunctionGraphTask::CreateAndDispatchWhenReady([=]()
		{
			Callback.Broadcast(outcome.IsSuccess(), outcome.GetResult(), static_cast<ES3Errors>(outcome.GetError().GetErrorType()), UTF8_TO_TCHAR(outcome.GetError().GetMessage().c_str()));
			SetReadyToDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	m_S3Object->GetAwsS3Client()->UploadPartCopyAsync(Request, CallbackLambda);
#endif
}
