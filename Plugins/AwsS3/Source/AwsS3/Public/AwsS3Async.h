/**
* Copyright (C) 2017-2023 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "AwsS3Types.h"
#include "AwsS3Library.h"
#include <Kismet/BlueprintAsyncActionBase.h>
#include "AwsS3Async.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetObject
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetObjectCallback, bool, bSuccess, const FGetObjectResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetObject : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetObjectCallback Callback;
public:
	/**
	*  Retrieves objects from Amazon S3.
	*
	* An Amazon S3 bucket has no directory hierarchy such as you would find in a typical computer file system. You can, however, create a logical hierarchy by using object key names that imply a folder structure. For example, instead of naming an object sample.jpg, you can name it photos/2006/February/sample.jpg.
	* To get an object from such a logical hierarchy, specify the full key name for the object in the GET operation. For a virtual hosted-style request example, if you have the object photos/2006/February/sample.jpg, specify the resource as /photos/2006/February/sample.jpg. For a path-style request example, if you have the object photos/2006/February/sample.jpg in the bucket named examplebucket, specify the resource as /examplebucket/photos/2006/February/sample.jpg. For more information about request types, see HTTP Host Header Bucket Specification.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetObject* GetObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetObjectRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3AbortMultipartUpload
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAbortMultipartUploadCallback, bool, bSuccess, const FAbortMultipartUploadResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3AbortMultipartUpload : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAbortMultipartUploadCallback Callback;
public:
	/**
	* This operation aborts a multipart upload. 
	*
	* After a multipart upload is aborted, no additional parts can be uploaded using that upload ID. The storage consumed by any previously uploaded parts will be freed. However, if any part uploads are currently in progress, those part uploads might or might not succeed. As a Result, it might be necessary to abort a given multipart upload multiple times in order to completely free all storage consumed by all parts.
	* To verify that all parts have been removed, so you don't get charged for the part storage, you should call the ListParts operation and ensure that the parts list is empty.
	* For information about permissions required to use the multipart upload API, see Multipart Upload API and Permissions.
	*
	* The following operations are related to AbortMultipartUpload:
	* CreateMultipartUpload
	* UploadPart
	* CompleteMultipartUpload
	* ListParts
	* ListMultipartUploads
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3AbortMultipartUpload* AbortMultipartUpload(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FAbortMultipartUploadRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FAbortMultipartUploadRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3CompleteMultipartUpload
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCompleteMultipartUploadCallback, bool, bSuccess, const FCompleteMultipartUploadResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3CompleteMultipartUpload : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnCompleteMultipartUploadCallback Callback;
public:
	/**
	* Completes a multipart upload by assembling previously uploaded parts.
	*
	* You first initiate the multipart upload and then upload all parts using the UploadPart operation. After successfully uploading all relevant parts of an upload, you call this operation to complete the upload. Upon receiving this request, Amazon S3 concatenates all the parts in ascending order by part number to create a new object. In the Complete Multipart Upload request, you must provide the parts list. You must ensure that the parts list is complete. This operation concatenates the parts that you provide in the list. For each part in the list, you must provide the part number and the ETag value, returned after that part was uploaded.
	* Processing of a Complete Multipart Upload request could take several minutes to complete. After Amazon S3 begins processing the request, it sends an HTTP response header that specifies a 200 OK response. While processing is in progress, Amazon S3 periodically sends white space characters to keep the connection from timing out. Because a request could fail after the initial 200 OK response has been sent, it is important that you check the response body to determine whether the request succeeded.
	* Note that if CompleteMultipartUpload fails, applications should be prepared to retry the failed requests. For more information, see Amazon S3 Error Best Practices.
	* For more information about multipart uploads, see Uploading Objects Using Multipart Upload.
	* For information about permissions required to use the multipart upload API, see Multipart Upload API and Permissions.
	* 
	* CompleteMultipartUpload has the following special errors:
	* Error code: EntityTooSmall
	* Description: Your proposed upload is smaller than the minimum allowed object size. Each part must be at least 5 MB in size, except the last part.
	* 400 Bad Request
	* Error code: InvalidPart
	* Description: One or more of the specified parts could not be found. The part might not have been uploaded, or the specified entity tag might not have matched the part's entity tag.
	* 400 Bad Request
	* Error code: InvalidPartOrder
	* Description: The list of parts was not in ascending order. The parts list must be specified in order by part number.
	* 400 Bad Request
	* Error code: NoSuchUpload
	* Description: The specified multipart upload does not exist. The upload ID might be invalid, or the multipart upload might have been aborted or completed.
	* 404 Not Found
	* 
	* The following operations are related to CompleteMultipartUpload:
	* CreateMultipartUpload
	* UploadPart
	* AbortMultipartUpload
	* ListParts
	* ListMultipartUploads
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3CompleteMultipartUpload* CompleteMultipartUpload(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FCompleteMultipartUploadRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FCompleteMultipartUploadRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3CopyObject
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCopyObjectCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3CopyObject : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnCopyObjectCallback Callback;
public:
	/**
	* Creates a copy of an object that is already stored in Amazon S3.
	*
	*All copy requests must be authenticated. Additionally, you must have read access to the source object and write access to the destination bucket. For more information, see REST Authentication. Both the Region that you want to copy the object from and the Region that you want to copy the object to must be enabled for your account.
	* A copy request might return an error when Amazon S3 receives the copy request or while Amazon S3 is copying the files. If the error occurs before the copy operation starts, you receive a standard Amazon S3 error. If the error occurs during the copy operation, the error response is embedded in the 200 OK response. This means that a 200 OK response can contain either a success or an error. Design your application to parse the contents of the response and handle it appropriately.
	* If the copy is successful, you receive a response with information about the copied object.
	*
	* The following operations are related to CopyObject:
	*
	* PutObject
	* GetObject
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3CopyObject* CopyObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FCopyObjectRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FCopyObjectRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3CreateBucket
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreateBucketCallback, bool, bSuccess, const FCreateBucketResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3CreateBucket : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnCreateBucketCallback Callback;
public:
	/**
	* Creates a new bucket. To create a bucket, you must register with Amazon S3 and have a valid AWS Access Key ID to authenticate requests. Anonymous requests are never allowed to create buckets. By creating the bucket, you become the bucket owner.
	*
	* Not every string is an acceptable bucket name. For information on bucket naming restrictions, see Working with Amazon S3 Buckets.
	* By default, the bucket is created in the US East (N. Virginia) Region. You can optionally specify a Region in the request body. You might choose a Region to optimize latency, minimize costs, or address regulatory requirements. For example, if you reside in Europe, you will probably find it advantageous to create buckets in the Europe (Ireland) Region. For more information, see How to Select a Region for Your Buckets.
	*
	* The following operations are related to CreateBucket:
	* PutObject
	* DeleteBucket
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3CreateBucket* CreateBucket(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FCreateBucketRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FCreateBucketRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3CreateMultipartUpload
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreateMultipartUploadCallback, bool, bSuccess, const FCreateMultipartUploadResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3CreateMultipartUpload : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnCreateMultipartUploadCallback Callback;
public:
	/**
	* This operation initiates a multipart upload and returns an upload ID. This upload ID is used to associate all of the parts in the specific multipart upload. You specify this upload ID in each of your subsequent upload part requests (see UploadPart). You also include this upload ID in the final request to either complete or abort the multipart upload request.
	* 
	* For more information about multipart uploads, see Multipart Upload Overview.
	* If you have configured a lifecycle rule to abort incomplete multipart uploads, the upload must complete within the number of days specified in the bucket lifecycle configuration. Otherwise, the incomplete multipart upload becomes eligible for an abort operation and Amazon S3 aborts the multipart upload. For more information, see Aborting Incomplete Multipart Uploads Using a Bucket Lifecycle Policy.
	* For information about the permissions required to use the multipart upload API, see Multipart Upload API and Permissions.
	* For request signing, multipart upload is just a series of regular requests. You initiate a multipart upload, send one or more requests to upload parts, and then complete the multipart upload process. You sign each request individually. There is nothing special about signing multipart upload requests. For more information about signing, see Authenticating Requests (AWS Signature Version 4).
	* You can optionally request server-side encryption. For server-side encryption, Amazon S3 encrypts your data as it writes it to disks in its data centers and decrypts it when you access it. You can provide your own encryption key, or use AWS Key Management Service (AWS KMS) customer master keys (CMKs) or Amazon S3-managed encryption keys. If you choose to provide your own encryption key, the request headers you provide in UploadPart and UploadPartCopy requests must match the headers you used in the request to initiate the upload by using CreateMultipartUpload.
	* To perform a multipart upload with encryption using an AWS KMS CMK, the requester must have permission to the kms:Encrypt, kms:Decrypt, kms:ReEncrypt*, kms:GenerateDataKey*, and kms:DescribeKey actions on the key. These permissions are required because Amazon S3 must decrypt and read data from the encrypted file parts before it completes the multipart upload.
	* If your AWS Identity and Access Management (IAM) user or role is in the same AWS account as the AWS KMS CMK, then you must have these permissions on the key policy. If your IAM user or role belongs to a different account than the key, then you must have the permissions on both the key policy and your IAM user or role.
	* For more information, see Protecting Data Using Server-Side Encryption.
	*
	* The following operations are related to CreateMultipartUpload:
	* UploadPart
	* CompleteMultipartUpload
	* AbortMultipartUpload
	* ListParts
	* ListMultipartUploads
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3CreateMultipartUpload* CreateMultipartUpload(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FCreateMultipartUploadRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FCreateMultipartUploadRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucket
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucket : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketCallback Callback;
public:
	/**
	* Deletes the bucket. All objects (including all object versions and delete markers) in the bucket must be deleted before the bucket itself can be deleted.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucket* DeleteBucket(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketAnalyticsConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketAnalyticsConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketAnalyticsConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketAnalyticsConfigurationCallback Callback;
public:
	/**
	* Deletes an analytics configuration for the bucket (specified by the analytics configuration ID).
	* 
	* To use this operation, you must have permissions to perform the s3:PutAnalyticsConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	* For information about the Amazon S3 analytics feature, see Amazon S3 Analytics � Storage Class Analysis.
	* 
	* The following operations are related to DeleteBucketAnalyticsConfiguration:
	* GetBucketAnalyticsConfiguration
	* ListBucketAnalyticsConfigurations
	* PutBucketAnalyticsConfiguration
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketAnalyticsConfiguration* DeleteBucketAnalyticsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketAnalyticsConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketAnalyticsConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketCors
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketCorsCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketCors : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketCorsCallback Callback;
public:
	/**
	* Deletes the cors configuration information set for the bucket.
	* 
	* To use this operation, you must have permission to perform the s3:PutBucketCORS action. The bucket owner has this permission by default and can grant this permission to others.
	* For information about cors, see Enabling Cross-Origin Resource Sharing in the Amazon Simple Storage Service Developer Guide.
	* 
	* Related Resources:
	* PutBucketCors
	* RESTOPTIONSobject
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketCors* DeleteBucketCors(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketCorsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketCorsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketEncryption
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketEncryptionCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketEncryption : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketEncryptionCallback Callback;
public:
	/**
	* This implementation of the DELETE operation removes default encryption from the bucket. For information about the Amazon S3 default encryption feature, see Amazon S3 Default Bucket Encryption in the Amazon Simple Storage Service Developer Guide.
	*
	* To use this operation, you must have permissions to perform the s3:PutEncryptionConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to your Amazon S3 Resources in the Amazon Simple Storage Service Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketEncryption* DeleteBucketEncryption(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketEncryptionRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketEncryptionRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketInventoryConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketInventoryConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketInventoryConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketInventoryConfigurationCallback Callback;
public:
	/**
	* Deletes an inventory configuration (identified by the inventory ID) from the bucket.
	* 
	* To use this operation, you must have permissions to perform the s3:PutInventoryConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	* For information about the Amazon S3 inventory feature, see Amazon S3 Inventory.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketInventoryConfiguration* DeleteBucketInventoryConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketInventoryConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketInventoryConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketLifecycle
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketLifecycleCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketLifecycle : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketLifecycleCallback Callback;
public:
	/**
	* Deletes the lifecycle configuration from the specified bucket. Amazon S3 removes all the lifecycle configuration rules in the lifecycle subresource associated with the bucket. Your objects never expire, and Amazon S3 no longer automatically deletes any objects on the basis of rules contained in the deleted lifecycle configuration.
	* 
	* To use this operation, you must have permission to perform the s3:PutLifecycleConfiguration action. By default, the bucket owner has this permission and the bucket owner can grant this permission to others.
	* There is usually some time lag before lifecycle configuration deletion is fully propagated to all the Amazon S3 systems.
	* For more information about the object expiration, see Elements to Describe Lifecycle Actions.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketLifecycle* DeleteBucketLifecycle(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketLifecycleRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketLifecycleRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketMetricsConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketMetricsConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketMetricsConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketMetricsConfigurationCallback Callback;
public:
	/**
	* Deletes a metrics configuration for the Amazon CloudWatch request metrics (specified by the metrics configuration ID) from the bucket. Note that this doesn't include the daily storage metrics.
	* 
	* To use this operation, you must have permissions to perform the s3:PutMetricsConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	* For information about CloudWatch request metrics for Amazon S3, see Monitoring Metrics with Amazon CloudWatch.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketMetricsConfiguration* DeleteBucketMetricsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketMetricsConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketMetricsConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketPolicy
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketPolicyCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketPolicy : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketPolicyCallback Callback;
public:
	/**
	* This implementation of the DELETE operation uses the policy subresource to delete the policy of a specified bucket. If you are using an identity other than the root user of the AWS account that owns the bucket, the calling identity must have the DeleteBucketPolicy permissions on the specified bucket and belong to the bucket owner's account to use this operation.
	*
	* If you don't have DeleteBucketPolicy permissions, Amazon S3 returns a 403 Access Denied error. If you have the correct permissions, but you're not using an identity that belongs to the bucket owner's account, Amazon S3 returns a 405 Method Not Allowed error.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketPolicy* DeleteBucketPolicy(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketPolicyRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketPolicyRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketReplication
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketReplicationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketReplication : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketReplicationCallback Callback;
public:
	/**
	* Deletes the replication configuration from the bucket.
	*
	* To use this operation, you must have permissions to perform the s3:PutReplicationConfiguration action. The bucket owner has these permissions by default and can grant it to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketReplication* DeleteBucketReplication(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketReplicationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketReplicationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketTagging
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketTaggingCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketTagging : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketTaggingCallback Callback;
public:
	/**
	* Deletes the tags from the bucket.
	*
	* To use this operation, you must have permission to perform the s3:PutBucketTagging action. By default, the bucket owner has this permission and can grant this permission to others.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketTagging* DeleteBucketTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketTaggingRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketTaggingRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteBucketWebsite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeleteBucketWebsiteCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteBucketWebsite : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteBucketWebsiteCallback Callback;
public:
	/**
	* This operation removes the website configuration for a bucket. Amazon S3 returns a 200 OK response upon successfully deleting a website configuration on the specified bucket. You will get a 200 OK response if the website configuration you are trying to delete does not exist on the bucket. Amazon S3 returns a 404 response if the bucket specified in the request does not exist.
	* 
	* This DELETE operation requires the S3:DeleteBucketWebsite permission. By default, only the bucket owner can delete the website configuration attached to a bucket. However, bucket owners can grant other users permission to delete the website configuration by writing a bucket policy granting them the S3:DeleteBucketWebsite permission.
	* For more information about hosting websites, see Hosting Websites on Amazon S3.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteBucketWebsite* DeleteBucketWebsite(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteBucketWebsiteRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteBucketWebsiteRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteObject
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDeleteObjectCallback, bool, bSuccess, const FDeleteObjectResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteObject : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteObjectCallback Callback;
public:
	/**
	* Removes the null version (if there is one) of an object and inserts a delete marker, which becomes the latest version of the object. If there isn't a null version, Amazon S3 does not remove any objects.
	* 
	* To remove a specific version, you must be the bucket owner and you must use the version Id subresource. Using this subresource permanently deletes the version. If the object deleted is a delete marker, Amazon S3 sets the response header, x-amz-delete-marker, to true.
	* If the object you want to delete is in a bucket where the bucket versioning configuration is MFA Delete enabled, you must include the x-amz-mfa request header in the DELETE versionId request. Requests that include x-amz-mfa must use HTTPS.
	* For more information about MFA Delete, see Using MFA Delete. To see sample requests that use versioning, see Sample Request.
	* You can delete objects by explicitly calling the DELETE Object API or configure its lifecycle (PutBucketLifecycle) to enable Amazon S3 to remove them for you. If you want to block users or accounts from removing or deleting objects from your bucket, you must deny them the s3:DeleteObject, s3:DeleteObjectVersion, and s3:PutLifeCycleConfiguration actions.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteObject* DeleteObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteObjectRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteObjectRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteObjectTagging
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDeleteObjectTaggingCallback, bool, bSuccess, const FDeleteObjectTaggingResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteObjectTagging : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteObjectTaggingCallback Callback;
public:
	/**
	* Removes the entire tag set from the specified object. For more information about managing object tags, see Object Tagging.
	* 
	* To use this operation, you must have permission to perform the s3:DeleteObjectTagging action.
	* To delete tags of a specific object version, add the versionId query parameter in the request. You will need permission for the s3:DeleteObjectVersionTagging action.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteObjectTagging* DeleteObjectTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteObjectTaggingRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteObjectTaggingRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeleteObjects
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDeleteObjectsCallback, bool, bSuccess, const FDeleteObjectsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeleteObjects : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteObjectsCallback Callback;
public:
	/**
	* This operation enables you to delete multiple objects from a bucket using a single HTTP request. If you know the object keys that you want to delete, then this operation provides a suitable alternative to sending individual delete requests, reducing per-request overhead.
	* 
	* The request contains a list of up to 1000 keys that you want to delete. In the XML, you provide the object key names, and optionally, version IDs if you want to delete a specific version of the object from a versioning-enabled bucket. For each key, Amazon S3 performs a delete operation and returns the Result of that delete, success, or failure, in the response. Note that if the object specified in the request is not found, Amazon S3 returns the Result as deleted.
	* The operation supports two modes for the response: verbose and quiet. By default, the operation uses verbose mode in which the response includes the Result of deletion of each key in your request. In quiet mode the response includes only keys where the delete operation encountered an error. For a successful deletion, the operation does not return any information about the delete in the response body.
	* When performing this operation on an MFA Delete enabled bucket, that attempts to delete any versioned objects, you must include an MFA token. If you do not provide one, the entire request will fail, even if there are non-versioned objects you are trying to delete. If you provide an invalid token, whether there are versioned keys in the request or not, the entire Multi-Object Delete request will fail. For information about MFA Delete, see MFA Delete.
	* Finally, the Content-MD5 header is required for all Multi-Object Delete requests. Amazon S3 uses the header value to ensure that your request body has not been altered in transit.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeleteObjects* DeleteObjects(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeleteObjectsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeleteObjectsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3DeletePublicAccessBlock
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeletePublicAccessBlockCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3DeletePublicAccessBlock : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeletePublicAccessBlockCallback Callback;
public:
	/**
	* Removes the PublicAccessBlock configuration for an Amazon S3 bucket.
	*
	* To use this operation, you must have the s3:PutBucketPublicAccessBlock permission. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3DeletePublicAccessBlock* DeletePublicAccessBlock(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FDeletePublicAccessBlockRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FDeletePublicAccessBlockRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketAccelerateConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketAccelerateConfigurationCallback, bool, bSuccess, const FGetBucketAccelerateConfigurationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketAccelerateConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketAccelerateConfigurationCallback Callback;
public:
	/**
	* This implementation of the GET operation uses the accelerate subresource to return the Transfer Acceleration state of a bucket, which is either Enabled or Suspended. Amazon S3 Transfer Acceleration is a bucket-level feature that enables you to perform faster data transfers to and from Amazon S3.
	* 
	* To use this operation, you must have permission to perform the s3:GetAccelerateConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to your Amazon S3 Resources in the Amazon Simple Storage Service Developer Guide.
	* You set the Transfer Acceleration state of an existing bucket to Enabled or Suspended by using the PutBucketAccelerateConfiguration operation.
	* A GET accelerate request does not return a state value for a bucket that has no transfer acceleration state. A bucket has no Transfer Acceleration state if a state has never been set on the bucket.
	* For more information about transfer acceleration, see Transfer Acceleration in the Amazon Simple Storage Service Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketAccelerateConfiguration* GetBucketAccelerateConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketAccelerateConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketAccelerateConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketAcl
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketAclCallback, bool, bSuccess, const FGetBucketAclResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketAcl : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketAclCallback Callback;
public:
	/**
	* This implementation of the GET operation uses the acl subresource to return the access control list (ACL) of a bucket.
	*
	* To use GET to return the ACL of the bucket, you must have READ_ACP access to the bucket. If READ_ACP permission is granted to the anonymous user, you can return the ACL of the bucket without using an authorization header.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketAcl* GetBucketAcl(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketAclRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketAclRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketAnalyticsConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketAnalyticsConfigurationCallback, bool, bSuccess, const FGetBucketAnalyticsConfigurationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketAnalyticsConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketAnalyticsConfigurationCallback Callback;
public:
	/**
	* This implementation of the GET operation returns an analytics configuration (identified by the analytics configuration ID) from the bucket.
	* 
	* To use this operation, you must have permissions to perform the s3:GetAnalyticsConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources in the Amazon Simple Storage Service Developer Guide.
	* For information about Amazon S3 analytics feature, see Amazon S3 Analytics � Storage Class Analysis in the Amazon Simple Storage Service Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketAnalyticsConfiguration* GetBucketAnalyticsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketAnalyticsConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketAnalyticsConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketCors
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketCorsCallback, bool, bSuccess, const FGetBucketCorsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketCors : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketCorsCallback Callback;
public:
	/**
	* Returns the cors configuration information set for the bucket.
	* 
	* To use this operation, you must have permission to perform the s3:GetBucketCORS action. By default, the bucket owner has this permission and can grant it to others.
	* For more information about cors, see Enabling Cross-Origin Resource Sharing.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketCors* GetBucketCors(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketCorsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketCorsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketEncryption
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketEncryptionCallback, bool, bSuccess, const FGetBucketEncryptionResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketEncryption : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketEncryptionCallback Callback;
public:
	/**
	* Returns the default encryption configuration for an Amazon S3 bucket. For information about the Amazon S3 default encryption feature, see Amazon S3 Default Bucket Encryption.
	* 
	* To use this operation, you must have permission to perform the s3:GetEncryptionConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketEncryption* GetBucketEncryption(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketEncryptionRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketEncryptionRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketInventoryConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketInventoryConfigurationCallback, bool, bSuccess, const FGetBucketInventoryConfigurationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketInventoryConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketInventoryConfigurationCallback Callback;
public:
	/**
	* Returns an inventory configuration (identified by the inventory configuration ID) from the bucket.
	* 
	* To use this operation, you must have permissions to perform the s3:GetInventoryConfiguration action. The bucket owner has this permission by default and can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	* For information about the Amazon S3 inventory feature, see Amazon S3 Inventory.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketInventoryConfiguration* GetBucketInventoryConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketInventoryConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketInventoryConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketLifecycleConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketLifecycleConfigurationCallback, bool, bSuccess, const FGetBucketLifecycleConfigurationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketLifecycleConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketLifecycleConfigurationCallback Callback;
public:
	/**
	* Returns the lifecycle configuration information set on the bucket. For information about lifecycle configuration, see Object Lifecycle Management.
	*
	* To use this operation, you must have permission to perform the s3:GetLifecycleConfiguration action. The bucket owner has this permission, by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketLifecycleConfiguration* GetBucketLifecycleConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketLifecycleConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketLifecycleConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketLocation
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketLocationCallback, bool, bSuccess, const FGetBucketLocationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketLocation : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketLocationCallback Callback;
public:
	/**
	* Returns the Region the bucket resides in. You set the bucket's Region using the LocationConstraint request parameter in a CreateBucket request. For more information, see CreateBucket.
	*
	* To use this implementation of the operation, you must be the bucket owner.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketLocation* GetBucketLocation(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketLocationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketLocationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketLogging
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketLoggingCallback, bool, bSuccess, const FGetBucketLoggingResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketLogging : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketLoggingCallback Callback;
public:
	/**
	* Returns the logging status of a bucket and the permissions users have to view and modify that status. To use GET, you must be the bucket owner.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketLogging* GetBucketLogging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketLoggingRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketLoggingRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketMetricsConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketMetricsConfigurationCallback, bool, bSuccess, const FGetBucketMetricsConfigurationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketMetricsConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketMetricsConfigurationCallback Callback;
public:
	/**
	* Gets a metrics configuration (specified by the metrics configuration ID) from the bucket. Note that this doesn't include the daily storage metrics.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketMetricsConfiguration* GetBucketMetricsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketMetricsConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketMetricsConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketNotificationConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketNotificationConfigurationCallback, bool, bSuccess, const FGetBucketNotificationConfigurationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketNotificationConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketNotificationConfigurationCallback Callback;
public:
	/**
	* No longer used, see GetBucketNotificationConfiguration.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketNotificationConfiguration* GetBucketNotificationConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketNotificationConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketNotificationConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketPolicy
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketPolicyCallback, bool, bSuccess, const FGetBucketPolicyResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketPolicy : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketPolicyCallback Callback;
public:
	/**
	*  Returns the policy of a specified bucket. If you are using an identity other than the root user of the AWS account that owns the bucket, the calling identity must have the GetBucketPolicy permissions on the specified bucket and belong to the bucket owner's account in order to use this operation.
	*
	* If you don't have GetBucketPolicy permissions, Amazon S3 returns a 403 Access Denied error. If you have the correct permissions, but you're not using an identity that belongs to the bucket owner's account, Amazon S3 returns a 405 Method Not Allowed error.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketPolicy* GetBucketPolicy(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketPolicyRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketPolicyRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketPolicyStatus
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketPolicyStatusCallback, bool, bSuccess, const FGetBucketPolicyStatusResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketPolicyStatus : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketPolicyStatusCallback Callback;
public:
	/**
	* Retrieves the policy status for an Amazon S3 bucket, indicating whether the bucket is public. In order to use this operation, you must have the s3:GetBucketPolicyStatus permission. For more information about Amazon S3 permissions, see Specifying Permissions in a Policy.
	*
	* For more information about when Amazon S3 considers a bucket public, see The Meaning of "Public".
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketPolicyStatus* GetBucketPolicyStatus(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketPolicyStatusRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketPolicyStatusRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketReplication
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketReplicationCallback, bool, bSuccess, const FGetBucketReplicationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketReplication : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketReplicationCallback Callback;
public:
	/**
	* Returns the replication configuration of a bucket.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketReplication* GetBucketReplication(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketReplicationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketReplicationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketRequestPayment
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketRequestPaymentCallback, bool, bSuccess, const FGetBucketRequestPaymentResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketRequestPayment : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketRequestPaymentCallback Callback;
public:
	/**
	* Returns the request payment configuration of a bucket. To use this version of the operation, you must be the bucket owner. For more information, see Requester Pays Buckets.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketRequestPayment* GetBucketRequestPayment(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketRequestPaymentRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketRequestPaymentRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketTagging
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketTaggingCallback, bool, bSuccess, const FGetBucketTaggingResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketTagging : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketTaggingCallback Callback;
public:
	/**
	* Returns the tag set associated with the bucket.
	*
	* To use this operation, you must have permission to perform the s3:GetBucketTagging action. By default, the bucket owner has this permission and can grant this permission to others.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketTagging* GetBucketTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketTaggingRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketTaggingRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketVersioning
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketVersioningCallback, bool, bSuccess, const FGetBucketVersioningResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketVersioning : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketVersioningCallback Callback;
public:
	/**
	* Returns the versioning state of a bucket.
	* 
	* To retrieve the versioning state of a bucket, you must be the bucket owner.
	* This implementation also returns the MFA Delete status of the versioning state. If the MFA Delete status is enabled, the bucket owner must use an authentication device to change the versioning state of the bucket.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketVersioning* GetBucketVersioning(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketVersioningRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketVersioningRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetBucketWebsite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetBucketWebsiteCallback, bool, bSuccess, const FGetBucketWebsiteResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetBucketWebsite : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetBucketWebsiteCallback Callback;
public:
	/**
	* Returns the website configuration for a bucket. To host website on Amazon S3, you can configure a bucket as website by adding a website configuration. For more information about hosting websites, see Hosting Websites on Amazon S3.
	* 
	* This GET operation requires the S3:GetBucketWebsite permission. By default, only the bucket owner can read the bucket website configuration. However, bucket owners can allow other users to read the website configuration by writing a bucket policy granting them the S3:GetBucketWebsite permission.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetBucketWebsite* GetBucketWebsite(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetBucketWebsiteRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetBucketWebsiteRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetObjectAcl
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetObjectAclCallback, bool, bSuccess, const FGetObjectAclResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetObjectAcl : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetObjectAclCallback Callback;
public:
	/**
	* Returns the access control list (ACL) of an object. To use this operation, you must have READ_ACP access to the object.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetObjectAcl* GetObjectAcl(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectAclRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetObjectAclRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetObjectLegalHold
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetObjectLegalHoldCallback, bool, bSuccess, const FGetObjectLegalHoldResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetObjectLegalHold : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetObjectLegalHoldCallback Callback;
public:
	/**
	* Gets an object's current Legal Hold status. For more information, see Locking Objects.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetObjectLegalHold* GetObjectLegalHold(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectLegalHoldRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetObjectLegalHoldRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetObjectLockConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetObjectLockConfigurationCallback, bool, bSuccess, const FGetObjectLockConfigurationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetObjectLockConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetObjectLockConfigurationCallback Callback;
public:
	/**
	* Gets the Object Lock configuration for a bucket. The rule specified in the Object Lock configuration will be applied by default to every new object placed in the specified bucket. For more information, see Locking Objects.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetObjectLockConfiguration* GetObjectLockConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectLockConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetObjectLockConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetObjectRetention
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetObjectRetentionCallback, bool, bSuccess, const FGetObjectRetentionResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetObjectRetention : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetObjectRetentionCallback Callback;
public:
	/**
	* Retrieves an object's retention settings. For more information, see Locking Objects.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetObjectRetention* GetObjectRetention(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectRetentionRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetObjectRetentionRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetObjectTagging
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetObjectTaggingCallback, bool, bSuccess, const FGetObjectTaggingResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetObjectTagging : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetObjectTaggingCallback Callback;
public:
	/**
	* Returns the tag-set of an object. You send the GET request against the tagging subresource associated with the object.
	* 
	* To use this operation, you must have permission to perform the s3:GetObjectTagging action. By default, the GET operation returns information about current version of an object. For a versioned bucket, you can have multiple versions of an object in your bucket. To retrieve tags of any other version, use the versionId query parameter. You also need permission for the s3:GetObjectVersionTagging action.
	* By default, the bucket owner has this permission and can grant this permission to others.
	* For information about the Amazon S3 object tagging feature, see Object Tagging.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetObjectTagging* GetObjectTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetObjectTaggingRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetObjectTaggingRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3GetPublicAccessBlock
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGetPublicAccessBlockCallback, bool, bSuccess, const FGetPublicAccessBlockResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3GetPublicAccessBlock : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGetPublicAccessBlockCallback Callback;
public:
	/**  
	* Retrieves the PublicAccessBlock configuration for an Amazon S3 bucket. To use this operation, you must have the s3:GetBucketPublicAccessBlock permission. For more information about Amazon S3 permissions, see Specifying Permissions in a Policy.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3GetPublicAccessBlock* GetPublicAccessBlock(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FGetPublicAccessBlockRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FGetPublicAccessBlockRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3HeadBucket
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHeadBucketCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3HeadBucket : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnHeadBucketCallback Callback;
public:
	/**
	* This operation is useful to determine if a bucket exists and you have permission to access it.
	*
	* The operation returns a 200 OK if the bucket exists and you have permission to access it. Otherwise, the operation might return responses such as 404 Not Found and 403 Forbidden.
	* To use this operation, you must have permissions to perform the s3:ListBucket action. The bucket owner has this permission by default and can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3HeadBucket* HeadBucket(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FHeadBucketRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FHeadBucketRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3HeadObject
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHeadObjectCallback, bool, bSuccess, const FHeadObjectResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3HeadObject : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnHeadObjectCallback Callback;
public:
	/**
	* The HEAD operation retrieves metadata from an object without returning the object itself. This operation is useful if you're only interested in an object's metadata. To use HEAD, you must have READ access to the object.
	* 
	* A HEAD request has the same options as a GET operation on an object. The response is identical to the GET response except that there is no response body.
	* If you encrypt an object by using server-side encryption with customer-provided encryption keys (SSE-C) when you store the object in Amazon S3, then when you retrieve the metadata from the object, you must use the following headers:
	* x-amz-server-side-encryption-customer-algorithm
	* x-amz-server-side-encryption-customer-key
	* x-amz-server-side-encryption-customer-key-MD5
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3HeadObject* HeadObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FHeadObjectRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FHeadObjectRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListBucketAnalyticsConfigurations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListBucketAnalyticsConfigurationsCallback, bool, bSuccess, const FListBucketAnalyticsConfigurationsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListBucketAnalyticsConfigurations : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListBucketAnalyticsConfigurationsCallback Callback;
public:
	/**
	* Lists the analytics configurations for the bucket. You can have up to 1,000 analytics configurations per bucket.
	* 
	* This operation supports list pagination and does not return more than 100 configurations at a time. You should always check the IsTruncated element in the response. If there are no more configurations to list, IsTruncated is set to false. If there are more configurations to list, IsTruncated is set to true, and there will be a value in NextContinuationToken. You use the NextContinuationToken value to continue the pagination of the list by passing the value in continuation-token in the request to GET the next page.
	* To use this operation, you must have permissions to perform the s3:GetAnalyticsConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	* For information about Amazon S3 analytics feature, see Amazon S3 Analytics � Storage Class Analysis.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListBucketAnalyticsConfigurations* ListBucketAnalyticsConfigurations(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListBucketAnalyticsConfigurationsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FListBucketAnalyticsConfigurationsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListBucketInventoryConfigurations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListBucketInventoryConfigurationsCallback, bool, bSuccess, const FListBucketInventoryConfigurationsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListBucketInventoryConfigurations : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListBucketInventoryConfigurationsCallback Callback;
public:
	/**
	* Returns a list of inventory configurations for the bucket. You can have up to 1,000 analytics configurations per bucket.
	* 
	* This operation supports list pagination and does not return more than 100 configurations at a time. Always check the IsTruncated element in the response. If there are no more configurations to list, IsTruncated is set to false. If there are more configurations to list, IsTruncated is set to true, and there is a value in NextContinuationToken. You use the NextContinuationToken value to continue the pagination of the list by passing the value in continuation-token in the request to GET the next page.
	* To use this operation, you must have permissions to perform the s3:GetInventoryConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	* For information about the Amazon S3 inventory feature, see Amazon S3 Inventory
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListBucketInventoryConfigurations* ListBucketInventoryConfigurations(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListBucketInventoryConfigurationsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FListBucketInventoryConfigurationsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListBucketMetricsConfigurations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListBucketMetricsConfigurationsCallback, bool, bSuccess, const FListBucketMetricsConfigurationsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListBucketMetricsConfigurations : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListBucketMetricsConfigurationsCallback Callback;
public:
	/**
	* Lists the metrics configurations for the bucket. The metrics configurations are only for the request metrics of the bucket and do not provide information on daily storage metrics. You can have up to 1,000 configurations per bucket.
	* 
	* This operation supports list pagination and does not return more than 100 configurations at a time. Always check the IsTruncated element in the response. If there are no more configurations to list, IsTruncated is set to false. If there are more configurations to list, IsTruncated is set to true, and there is a value in NextContinuationToken. You use the NextContinuationToken value to continue the pagination of the list by passing the value in continuation-token in the request to GET the next page.
	* To use this operation, you must have permissions to perform the s3:GetMetricsConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListBucketMetricsConfigurations* ListBucketMetricsConfigurations(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListBucketMetricsConfigurationsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FListBucketMetricsConfigurationsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListBuckets
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListBucketsCallback, bool, bSuccess, const FListBucketsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListBuckets : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListBucketsCallback Callback;
public:
	/**
	* Returns a list of all buckets owned by the authenticated sender of the request.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListBuckets* ListBuckets(UObject* WorldContextObject, UAwsS3ClientObject* S3object);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListMultipartUploads
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListMultipartUploadsCallback, bool, bSuccess, const FListMultipartUploadsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListMultipartUploads : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListMultipartUploadsCallback Callback;
public:
	/**
	* This operation lists in-progress multipart uploads.
	* 
	* An in-progress multipart upload is a multipart upload that has been initiated using the Initiate Multipart Upload request, but has not yet been completed or aborted.
	* This operation returns at most 1,000 multipart uploads in the response. 1,000 multipart uploads is the maximum number of uploads a response can include, which is also the default value. You can further limit the number of uploads in a response by specifying the max-uploads parameter in the response. If additional multipart uploads satisfy the list criteria, the response will contain an IsTruncated element with the value true. To list the additional multipart uploads, use the key-marker and upload-id-marker request parameters.
	* In the response, the uploads are sorted by key. If your application has initiated more than one multipart upload using the same object key, then uploads in the response are first sorted by key. Additionally, uploads are sorted in ascending order within each key by the upload initiation time.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListMultipartUploads* ListMultipartUploads(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListMultipartUploadsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FListMultipartUploadsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListObjectVersions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListObjectVersionsCallback, bool, bSuccess, const FListObjectVersionsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListObjectVersions : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListObjectVersionsCallback Callback;
public:
	/**
	* Returns metadata about all of the versions of objects in a bucket. You can also use request parameters as selection criteria to return metadata about a subset of all the object versions.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListObjectVersions* ListObjectVersions(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListObjectVersionsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FListObjectVersionsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListObjects
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListObjectsCallback, bool, bSuccess, const FListObjectsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListObjects : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListObjectsCallback Callback;
public:
	/**
	* Returns some or all (up to 1,000) of the objects in a bucket. You can use the request parameters as selection criteria to return a subset of the objects in a bucket. A 200 OK response can contain valid or invalid XML. Be sure to design your application to parse the contents of the response and handle it appropriately.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListObjects* ListObjects(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListObjectsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FListObjectsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListObjectsV2
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListObjectsV2Callback, bool, bSuccess, const FListObjectsV2Result&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListObjectsV2 : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListObjectsV2Callback Callback;
public:
	/**
	* Returns some or all (up to 1,000) of the objects in a bucket. You can use the request parameters as selection criteria to return a subset of the objects in a bucket. A 200 OK response can contain valid or invalid XML. Make sure to design your application to parse the contents of the response and handle it appropriately.
	* 
	* To use this operation, you must have READ access to the bucket.
	* To use this operation in an AWS Identity and Access Management (IAM) policy, you must have permissions to perform the s3:ListBucket action. The bucket owner has this permission by default and can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListObjectsV2* ListObjectsV2(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListObjectsV2Request& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FListObjectsV2Request m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3ListParts
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnListPartsCallback, bool, bSuccess, const FListPartsResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3ListParts : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnListPartsCallback Callback;
public:
	/**
	* Lists the parts that have been uploaded for a specific multipart upload. This operation must include the upload ID, which you obtain by sending the initiate multipart upload request (see CreateMultipartUpload). This request returns a maximum of 1,000 uploaded parts. The default number of parts returned is 1,000 parts. You can restrict the number of parts returned by specifying the max-parts request parameter. If your multipart upload consists of more than 1,000 parts, the response returns an IsTruncated field with the value of true, and a NextPartNumberMarker element. In subsequent ListParts requests you can include the part-number-marker query string parameter and set its value to the NextPartNumberMarker field value from the previous response.
	* 
	* For more information on multipart uploads, see Uploading Objects Using Multipart Upload.
	* For information on permissions required to use the multipart upload API, see Multipart Upload API and Permissions.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3ListParts* ListParts(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FListPartsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FListPartsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketAccelerateConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketAccelerateConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketAccelerateConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketAccelerateConfigurationCallback Callback;
public:
	/**
	* Sets the accelerate configuration of an existing bucket. 
	* 	
	* Amazon S3 Transfer Acceleration is a bucket-level feature that enables you to perform faster data transfers to Amazon S3.
	* To use this operation, you must have permission to perform the s3:PutAccelerateConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketAccelerateConfiguration* PutBucketAccelerateConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketAccelerateConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketAccelerateConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketAcl
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketAclCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketAcl : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketAclCallback Callback;
public:
	/**
	* Sets the permissions on an existing bucket using access control lists (ACL). For more information, see Using ACLs. To set the ACL of a bucket, you must have WRITE_ACP permission.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketAcl* PutBucketAcl(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketAclRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketAclRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketAnalyticsConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketAnalyticsConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketAnalyticsConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketAnalyticsConfigurationCallback Callback;
public:
	/**
	* Sets an analytics configuration for the bucket (specified by the analytics configuration ID). You can have up to 1,000 analytics configurations per bucket.
	*
	* You can choose to have storage class analysis export analysis reports sent to a comma-separated values (CSV) flat file. See the DataExport request element. Reports are updated daily and are based on the object filters that you configure. When selecting data export, you specify a destination bucket and an optional destination prefix where the file is written. You can export the data to a destination bucket in a different account. However, the destination bucket must be in the same Region as the bucket that you are making the PUT analytics configuration to. For more information, see Amazon S3 Analytics � Storage Class Analysis.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketAnalyticsConfiguration* PutBucketAnalyticsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketAnalyticsConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketAnalyticsConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketCors
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketCorsCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketCors : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketCorsCallback Callback;
public:
	/**
	* Sets the cors configuration for your bucket. If the configuration exists, Amazon S3 replaces it.
	* 
	* To use this operation, you must be allowed to perform the s3:PutBucketCORS action. By default, the bucket owner has this permission and can grant it to others.
	* You set this configuration on a bucket so that the bucket can service cross-origin requests. For example, you might want to enable a request whose origin is http://www.example.com to access your Amazon S3 bucket at my.example.bucket.com by using the browser's XMLHttpRequest capability.
	* To enable cross-origin resource sharing (CORS) on a bucket, you add the cors subresource to the bucket. The cors subresource is an XML document in which you configure rules that identify origins and the HTTP methods that can be executed on your bucket. The document is limited to 64 KB in size.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketCors* PutBucketCors(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketCorsRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketCorsRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketEncryption
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketEncryptionCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketEncryption : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketEncryptionCallback Callback;
public:
	/** 
	* This implementation of the PUT operation uses the encryption subresource to set the default encryption state of an existing bucket.
	*
	* This implementation of the PUT operation sets default encryption for a bucket using server-side encryption with Amazon S3-managed keys SSE-S3 or AWS KMS customer master keys (CMKs) (SSE-KMS). For information about the Amazon S3 default encryption feature, see Amazon S3 Default Bucket Encryption.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketEncryption* PutBucketEncryption(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketEncryptionRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketEncryptionRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketInventoryConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketInventoryConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketInventoryConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketInventoryConfigurationCallback Callback;
public:
	/**
	* This implementation of the PUT operation adds an inventory configuration (identified by the inventory ID) to the bucket. You can have up to 1,000 inventory configurations per bucket.
	* 
	* Amazon S3 inventory generates inventories of the objects in the bucket on a daily or weekly basis, and the Results are published to a flat file. The bucket that is inventoried is called the source bucket, and the bucket where the inventory flat file is stored is called the destination bucket. The destination bucket must be in the same AWS Region as the source bucket.
	* When you configure an inventory for a source bucket, you specify the destination bucket where you want the inventory to be stored, and whether to generate the inventory daily or weekly. You can also configure what object metadata to include and whether to inventory all object versions or only current versions. For more information, see Amazon S3 Inventory in the Amazon Simple Storage Service Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketInventoryConfiguration* PutBucketInventoryConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketInventoryConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketInventoryConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketLifecycleConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketLifecycleConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketLifecycleConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketLifecycleConfigurationCallback Callback;
public:
	/**
	* Creates a new lifecycle configuration for the bucket or replaces an existing lifecycle configuration. For information about lifecycle configuration, see Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketLifecycleConfiguration* PutBucketLifecycleConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketLifecycleConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketLifecycleConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketLogging
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketLoggingCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketLogging : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketLoggingCallback Callback;
public:
	/**
	* Set the logging parameters for a bucket and to specify permissions for who can view and modify the logging parameters. All logs are saved to buckets in the same AWS Region as the source bucket. To set the logging status of a bucket, you must be the bucket owner.
	*
	* The bucket owner is automatically granted FULL_CONTROL to all logs. You use the Grantee request element to grant access to other people. The Permissions request element specifies the kind of access the grantee has to the logs.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketLogging* PutBucketLogging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketLoggingRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketLoggingRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketMetricsConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketMetricsConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketMetricsConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketMetricsConfigurationCallback Callback;
public:
	/**
	* Sets a metrics configuration (specified by the metrics configuration ID) for the bucket. 
	* 	
	* You can have up to 1,000 metrics configurations per bucket. If you're updating an existing metrics configuration, note that this is a full replacement of the existing metrics configuration. If you don't include the elements you want to keep, they are erased.
	* To use this operation, you must have permissions to perform the s3:PutMetricsConfiguration action. The bucket owner has this permission by default. The bucket owner can grant this permission to others. For more information about permissions, see Permissions Related to Bucket Subresource Operations and Managing Access Permissions to Your Amazon S3 Resources.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketMetricsConfiguration* PutBucketMetricsConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketMetricsConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketMetricsConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketNotificationConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketNotificationConfigurationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketNotificationConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketNotificationConfigurationCallback Callback;
public:
	/** 
	* Enables notifications of specified events for a bucket. For more information about event notifications, see Configuring Event Notifications.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketNotificationConfiguration* PutBucketNotificationConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketNotificationConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketNotificationConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketPolicy
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketPolicyCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketPolicy : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketPolicyCallback Callback;
public:
	/**
	* Applies an Amazon S3 bucket policy to an Amazon S3 bucket.
	* 
	* If you are using an identity other than the root user of the AWS account that owns the bucket, the calling identity must have the PutBucketPolicy permissions on the specified bucket and belong to the bucket owner's account in order to use this operation.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketPolicy* PutBucketPolicy(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketPolicyRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketPolicyRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketReplication
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketReplicationCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketReplication : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketReplicationCallback Callback;
public:
	/**
	* Creates a replication configuration or replaces an existing one. For more information, see Replication in the Amazon S3 Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketReplication* PutBucketReplication(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketReplicationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketReplicationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketRequestPayment
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketRequestPaymentCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketRequestPayment : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketRequestPaymentCallback Callback;
public:
	/**
	* Sets the request payment configuration for a bucket. By default, the bucket owner pays for downloads from the bucket. This configuration parameter enables the bucket owner (only) to specify that the person requesting the download will be charged for the download. For more information, see Requester Pays Buckets.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketRequestPayment* PutBucketRequestPayment(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketRequestPaymentRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketRequestPaymentRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketTagging
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketTaggingCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketTagging : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketTaggingCallback Callback;
public:
	/**
	* Sets the tags for a bucket.

Use tags to organize your AWS bill to reflect your own cost structure. To do this, sign up to get your AWS account bill with tag key values included. Then, to see the cost of combined resources, organize your billing information according to resources with the same tag key values. For example, you can tag several resources with a specific application name, and then organize your billing information to see the total cost of that application across several services. For more information, see Cost Allocation and Tagging.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketTagging* PutBucketTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketTaggingRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketTaggingRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketVersioning
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketVersioningCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketVersioning : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketVersioningCallback Callback;
public:
	/**
	* Sets the versioning state of an existing bucket. To set the versioning state, you must be the bucket owner.
	* 
	* You can set the versioning state with one of the following values:
	* Enabled�Enables versioning for the objects in the bucket. All objects added to the bucket receive a unique version ID.
	* Suspended�Disables versioning for the objects in the bucket. All objects added to the bucket receive the version ID null.
	* If the versioning state has never been set on a bucket, it has no versioning state; a GetBucketVersioning request does not return a versioning state value.
	* If the bucket owner enables MFA Delete in the bucket versioning configuration, the bucket owner must include the x-amz-mfa request header and the Status and the MfaDelete request elements in a request to set the versioning state of the bucket.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketVersioning* PutBucketVersioning(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketVersioningRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketVersioningRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutBucketWebsite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutBucketWebsiteCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutBucketWebsite : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutBucketWebsiteCallback Callback;
public:
	/**
	* Sets the configuration of the website that is specified in the website subresource. To configure a bucket as a website, you can add this subresource on the bucket with website configuration information such as the file name of the index document and any redirect rules. For more information, see Hosting Websites on Amazon S3.
	* 
	* This PUT operation requires the S3:PutBucketWebsite permission. By default, only the bucket owner can configure the website attached to a bucket; however, bucket owners can allow other users to set the website configuration by writing a bucket policy that grants them the S3:PutBucketWebsite permission.
	* 
	* To redirect all website requests sent to the bucket's website endpoint, you add a website configuration with the following elements. Because all requests are sent to another website, you don't need to provide index document name for the bucket.
	* WebsiteConfiguration
	* RedirectAllRequestsTo
	* HostName
	* Protocol
	* 
	* If you want granular control over redirects, you can use the following elements to add routing rules that describe conditions for redirecting requests and information about the redirect destination. In this case, the website configuration must provide an index document for the bucket, because some requests might not be redirected.
	* WebsiteConfiguration
	* IndexDocument
	* Suffix
	* ErrorDocument
	* Key
	* RoutingRules
	* RoutingRule
	* Condition
	* HttpErrorCodeReturnedEquals
	* KeyPrefixEquals
	* Redirect
	* Protocol
	* HostName
	* ReplaceKeyPrefixWith
	* ReplaceKeyWith
	* HttpRedirectCode
	* 
	* Amazon S3 has a limitation of 50 routing rules per website configuration. If you require more than 50 routing rules, you can use object redirect. For more information, see Configuring an Object Redirect in the Amazon Simple Storage Service Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutBucketWebsite* PutBucketWebsite(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutBucketWebsiteRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutBucketWebsiteRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutObject
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPutObjectCallback, bool, bSuccess, const FPutObjectResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutObject : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutObjectCallback Callback;
public:
	/**
	* Adds an object to a bucket. You must have WRITE permissions on a bucket to add an object to it.
	* 
	* Amazon S3 never adds partial objects; if you receive a success response, Amazon S3 added the entire object to the bucket.
	* Amazon S3 is a distributed system. If it receives multiple write requests for the same object simultaneously, it overwrites all but the last object written. Amazon S3 does not provide object locking; if you need this, make sure to build it into your application layer or use versioning instead.
	* To ensure that data is not corrupted traversing the network, use the Content-MD5 header. When you use this header, Amazon S3 checks the object against the provided MD5 value and, if they do not match, returns an error. Additionally, you can calculate the MD5 while putting an object to Amazon S3 and compare the returned ETag to the calculated MD5 value.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutObject* PutObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutObjectRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutObjectAcl
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPutObjectAclCallback, bool, bSuccess, const FPutObjectAclResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutObjectAcl : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutObjectAclCallback Callback;
public:
	/**
	* Uses the acl subresource to set the access control list (ACL) permissions for a new or existing object in an S3 bucket. You must have WRITE_ACP permission to set the ACL of an object. For more information, see What permissions can I grant? in the Amazon Simple Storage Service Developer Guide.
	*
	* Depending on your application needs, you can choose to set the ACL on an object using either the request body or the headers. For example, if you have an existing application that updates a bucket ACL using the request body, you can continue to use that approach. For more information, see Access Control List (ACL) Overview in the Amazon S3 Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutObjectAcl* PutObjectAcl(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectAclRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutObjectAclRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutObjectLegalHold
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPutObjectLegalHoldCallback, bool, bSuccess, const FPutObjectLegalHoldResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutObjectLegalHold : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutObjectLegalHoldCallback Callback;
public:
	/**
	* Applies a Legal Hold configuration to the specified object.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutObjectLegalHold* PutObjectLegalHold(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectLegalHoldRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutObjectLegalHoldRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutObjectLockConfiguration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPutObjectLockConfigurationCallback, bool, bSuccess, const FPutObjectLockConfigurationResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutObjectLockConfiguration : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutObjectLockConfigurationCallback Callback;
public:
	/**
	* Places an Object Lock configuration on the specified bucket. The rule specified in the Object Lock configuration will be applied by default to every new object placed in the specified bucket.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutObjectLockConfiguration* PutObjectLockConfiguration(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectLockConfigurationRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutObjectLockConfigurationRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutObjectRetention
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPutObjectRetentionCallback, bool, bSuccess, const FPutObjectRetentionResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutObjectRetention : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutObjectRetentionCallback Callback;
public:
	/**
	* Places an Object Retention configuration on an object.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutObjectRetention* PutObjectRetention(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectRetentionRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutObjectRetentionRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutObjectTagging
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPutObjectTaggingCallback, bool, bSuccess, const FPutObjectTaggingResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutObjectTagging : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutObjectTaggingCallback Callback;
public:
	/**
	* Sets the supplied tag-set to an object that already exists in a bucket.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutObjectTagging* PutObjectTagging(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutObjectTaggingRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutObjectTaggingRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3PutPublicAccessBlock
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPutPublicAccessBlockCallback, bool, bSuccess, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3PutPublicAccessBlock : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPutPublicAccessBlockCallback Callback;
public:
	/**
	* Creates or modifies the PublicAccessBlock configuration for an Amazon S3 bucket. To use this operation, you must have the s3:PutBucketPublicAccessBlock permission. For more information about Amazon S3 permissions, see Specifying Permissions in a Policy.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3PutPublicAccessBlock* PutPublicAccessBlock(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FPutPublicAccessBlockRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FPutPublicAccessBlockRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3RestoreObject
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRestoreObjectCallback, bool, bSuccess, const FRestoreObjectResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3RestoreObject : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnRestoreObjectCallback Callback;
public:
	/**
	* Restores an archived copy of an object back into Amazon S3
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3RestoreObject* RestoreObject(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FRestoreObjectRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FRestoreObjectRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3UploadPart
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUploadPartCallback, bool, bSuccess, const FUploadPartResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3UploadPart : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUploadPartCallback Callback;
public:
	/**
	* Uploads a part in a multipart upload.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3UploadPart* UploadPart(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FUploadPartRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FUploadPartRequest m_RequestData;
private:
	virtual void Activate() override;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		US3UploadPartCopy
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUploadPartCopyCallback, bool, bSuccess, const FUploadPartCopyResult&, Result, ES3Errors, ErrorType, const FString&, ErrorMessage);

UCLASS()
class US3UploadPartCopy : public US3Async
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnUploadPartCopyCallback Callback;
public:
	/**
	* Uploads a part by copying data from an existing object as data source. You specify the data source by adding the request header x-amz-copy-source in your request and a byte range by adding the request header x-amz-copy-source-range in your request.
	*
	* The minimum allowable part size for a multipart upload is 5 MB. For more information about multipart upload limits, go to Quick Facts in the Amazon Simple Storage Service Developer Guide.
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", Category = "AWS|S3", BlueprintInternalUseOnly="true"))
	static US3UploadPartCopy* UploadPartCopy(UObject* WorldContextObject, UAwsS3ClientObject* S3object, const FUploadPartCopyRequest& Request);
protected:
	UPROPERTY()
	UAwsS3ClientObject* m_S3Object;
	FUploadPartCopyRequest m_RequestData;
private:
	virtual void Activate() override;
};
