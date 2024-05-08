/**
* Copyright (C) 2017-2023 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#if !DISABLE_S3
THIRD_PARTY_INCLUDES_START
#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/SelectObjectContentHandler.h>
#include <aws/core/utils/event/EventStreamDecoder.h>
#include <aws/s3/model/CompletedPart.h>
#include <aws/s3/model/CompletedMultipartUpload.h>
#include <aws/s3/model/CreateBucketConfiguration.h>
#include <aws/s3/model/ObjectIdentifier.h>
#include <aws/s3/model/Delete.h>
#include <aws/s3/model/AccelerateConfiguration.h>
#include <aws/s3/model/Grantee.h>
#include <aws/s3/model/AccessControlPolicy.h>
#include <aws/s3/model/Owner.h>
#include <aws/s3/model/CORSConfiguration.h>
#include <aws/s3/model/ServerSideEncryptionConfiguration.h>
#include <aws/s3/model/AnalyticsFilter.h>
#include <aws/s3/model/AnalyticsAndOperator.h>
#include <aws/s3/model/AnalyticsConfiguration.h>
#include <aws/s3/model/StorageClassAnalysis.h>
#include <aws/s3/model/StorageClassAnalysisDataExport.h>
#include <aws/s3/model/AnalyticsS3ExportFileFormat.h>
#include <aws/s3/model/InventoryConfiguration.h>
#include <aws/s3/model/InventoryDestination.h>
#include <aws/s3/model/InventoryFilter.h>
#include <aws/s3/model/InventorySchedule.h>
#include <aws/s3/model/InventoryS3BucketDestination.h>
#include <aws/s3/model/InventoryEncryption.h>
#include <aws/s3/model/InventoryIncludedObjectVersions.h>
#include <aws/s3/model/BucketLifecycleConfiguration.h>
#include <aws/s3/model/LifecycleRule.h>
#include <aws/s3/model/LifecycleExpiration.h>
#include <aws/s3/model/LifecycleRuleFilter.h>
#include <aws/s3/model/LifecycleRuleAndOperator.h>
#include <aws/s3/model/Transition.h>
#include <aws/s3/model/NoncurrentVersionTransition.h>
#include <aws/s3/model/NoncurrentVersionExpiration.h>
#include <aws/s3/model/TransitionStorageClass.h>
#include <aws/s3/model/BucketLoggingStatus.h>
#include <aws/s3/model/TargetGrant.h>
#include <aws/s3/model/MetricsConfiguration.h>
#include <aws/s3/model/MetricsAndOperator.h>
#include <aws/s3/model/NotificationConfiguration.h>
#include <aws/s3/model/TopicConfiguration.h>
#include <aws/s3/model/NotificationConfigurationFilter.h>
#include <aws/s3/model/S3KeyFilter.h>
#include <aws/s3/model/FilterRule.h>
#include <aws/s3/model/LambdaFunctionConfiguration.h>
#include <aws/s3/model/ReplicationConfiguration.h>
#include <aws/s3/model/ReplicationRule.h>
#include <aws/s3/model/ReplicationRuleFilter.h>
#include <aws/s3/model/ReplicationRuleAndOperator.h>
#include <aws/s3/model/SourceSelectionCriteria.h>
#include <aws/s3/model/SseKmsEncryptedObjects.h>
#include <aws/s3/model/ExistingObjectReplication.h>
#include <aws/s3/model/Destination.h>
#include <aws/s3/model/AccessControlTranslation.h>
#include <aws/s3/model/EncryptionConfiguration.h>
#include <aws/s3/model/ReplicationTime.h>
#include <aws/s3/model/Metrics.h>
#include <aws/s3/model/DeleteMarkerReplication.h>
#include <aws/s3/model/RequestPaymentConfiguration.h>
#include <aws/s3/model/Tagging.h>
#include <aws/s3/model/VersioningConfiguration.h>
#include <aws/s3/model/WebsiteConfiguration.h>
#include <aws/s3/model/ErrorDocument.h>
#include <aws/s3/model/IndexDocument.h>
#include <aws/s3/model/RedirectAllRequestsTo.h>
#include <aws/s3/model/RoutingRule.h>
#include <aws/s3/model/Condition.h>
#include <aws/s3/model/Redirect.h>
#include <aws/s3/model/ObjectLockLegalHold.h>
#include <aws/s3/model/ObjectLockConfiguration.h>
#include <aws/s3/model/ObjectLockRule.h>
#include <aws/s3/model/DefaultRetention.h>
#include <aws/s3/model/ObjectLockRetention.h>
#include <aws/s3/model/PublicAccessBlockConfiguration.h>
#include <aws/s3/model/RestoreRequest.h>
#include <aws/s3/model/GlacierJobParameters.h>
#include <aws/s3/model/SelectParameters.h>
#include <aws/s3/model/InputSerialization.h>
#include <aws/s3/model/CSVInput.h>
#include <aws/s3/model/JSONInput.h>
#include <aws/s3/model/OutputLocation.h>
#include <aws/s3/model/Encryption.h>
#include <aws/s3/model/MetadataEntry.h>
#include <aws/s3/model/OutputSerialization.h>
#include <aws/s3/model/JSONOutput.h>
#include <aws/core/auth/AWSCredentials.h>
THIRD_PARTY_INCLUDES_END
#endif
#include "AwsS3Types.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAwsS3, Log, All);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

UENUM(BlueprintType)
enum class EPayloadSigningPolicy : uint8
{
	/**
	 * Sign the request based on the value returned by AmazonWebServiceRequest::SignBody()
	 */
	RequestDependent,
	/**
	 * Always sign the body of the request.
	 */
	Always,
	/**
	 * Never sign the body of the request
	 */
	Never
};

UENUM(BlueprintType)
enum class EChecksumMode : uint8
{
	NOT_SET,
	ENABLED
};

UENUM(BlueprintType)
enum class EChecksumAlgorithm : uint8
{
	NOT_SET,
	CRC32,
	CRC32C,
	SHA1,
	SHA256
};

UENUM(BlueprintType)
enum class EQuoteFields : uint8
{
	NOT_SET,
	ALWAYS,
	ASNEEDED
};

UENUM(BlueprintType)
enum class ES3ExpressionType : uint8
{
	NOT_SET,
	SQL
};

UENUM(BlueprintType)
enum class EJSONType : uint8
{
	NOT_SET,
	DOCUMENT,
	LINES
};

UENUM(BlueprintType)
enum class ECompressionType : uint8
{
	NOT_SET,
	NONE,
	GZIP,
	BZIP2
};

UENUM(BlueprintType)
enum class EFileHeaderInfo : uint8
{
	NOT_SET,
	USE,
	IGNORE,
	NONE
};

UENUM(BlueprintType)
enum class ERestoreRequestType : uint8
{
	NOT_SET,
	SELECT
};

UENUM(BlueprintType)
enum class ETier : uint8
{
	NOT_SET,
	Standard,
	Bulk,
	Expedited
};

UENUM(BlueprintType)
enum class ERequestCharged : uint8
{
	NOT_SET,
	requester
};

UENUM(BlueprintType)
enum class EMFADeleteStatus : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class EObjectAttributes : uint8
{
	NOT_SET,
	ETag,
	Checksum,
	ObjectParts,
	StorageClass,
	ObjectSize
};

UENUM(BlueprintType)
enum class EReplicationStatus : uint8
{
	NOT_SET,
	COMPLETED,
	PENDING,
	FAILED,
	REPLICA
};

UENUM(BlueprintType)
enum class EObjectLockEnabled : uint8
{
	NOT_SET,
	Enabled
};

UENUM(BlueprintType)
enum class EObjectLockRetentionMode : uint8
{
	NOT_SET,
	GOVERNANCE,
	COMPLIANCE
};

UENUM(BlueprintType)
enum class EObjectVersionStorageClass : uint8
{
	NOT_SET,
	STANDARD
};

UENUM(BlueprintType)
enum class EObjectStorageClass : uint8
{
	NOT_SET,
	STANDARD,
	REDUCED_REDUNDANCY,
	GLACIER,
	STANDARD_IA,
	ONEZONE_IA,
	INTELLIGENT_TIERING,
	DEEP_ARCHIVE
};

UENUM(BlueprintType)
enum class ES3ComparisonOperator : uint8
{
	NOT_SET,
	EQ,
	NE,
	INN,
	LE,
	LT,
	GE,
	GT,
	BETWEEN,
	NOT_NULL,
	NULL_,
	CONTAINS,
	NOT_CONTAINS,
	BEGINS_WITH
};

UENUM(BlueprintType)
enum class ES3Protocol : uint8
{
	NOT_SET,
	http,
	https
};

UENUM(BlueprintType)
enum class EBucketVersioningStatus : uint8
{
	NOT_SET,
	Enabled,
	Suspended
};

UENUM(BlueprintType)
enum class EMFADelete : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class EPayer : uint8
{
	NOT_SET,
	Requester,
	BucketOwner
};

UENUM(BlueprintType)
enum class EDeleteMarkerReplicationStatus : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class EMetricsStatus : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class EReplicationTimeStatus : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class EOwnerOverride : uint8
{
	NOT_SET,
	Destination
};

UENUM(BlueprintType)
enum class EStorageClass : uint8
{
	NOT_SET,
	STANDARD,
	REDUCED_REDUNDANCY,
	STANDARD_IA,
	ONEZONE_IA,
	INTELLIGENT_TIERING,
	GLACIER,
	DEEP_ARCHIVE
};

UENUM(BlueprintType)
enum class EExistingObjectReplicationStatus : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class ESseKmsEncryptedObjectsStatus : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class EReplicationRuleStatus : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class ES3Event : uint8
{
	NOT_SET,
	s3_ReducedRedundancyLostObject,
	s3_ObjectCreated,
	s3_ObjectCreated_Put,
	s3_ObjectCreated_Post,
	s3_ObjectCreated_Copy,
	s3_ObjectCreated_CompleteMultipartUpload,
	s3_ObjectRemoved,
	s3_ObjectRemoved_Delete,
	s3_ObjectRemoved_DeleteMarkerCreated,
	s3_ObjectRestore,
	s3_ObjectRestore_Post,
	s3_ObjectRestore_Completed,
	s3_Replication,
	s3_Replication_OperationFailedReplication,
	s3_Replication_OperationNotTracked,
	s3_Replication_OperationMissedThreshold,
	s3_Replication_OperationReplicatedAfterThreshold
};

UENUM(BlueprintType)
enum class EFilterRuleName : uint8
{
	NOT_SET,
	prefix,
	suffix
};

UENUM(BlueprintType)
enum class EBucketLogsPermission : uint8
{
	NOT_SET,
	FULL_CONTROL,
	READ,
	WRITE
};

UENUM(BlueprintType)
enum class ETransitionStorageClass : uint8
{
	NOT_SET,
	GLACIER,
	STANDARD_IA,
	ONEZONE_IA,
	INTELLIGENT_TIERING,
	DEEP_ARCHIVE
};

UENUM(BlueprintType)
enum class EExpirationStatus : uint8
{
	NOT_SET,
	Enabled,
	Disabled
};

UENUM(BlueprintType)
enum class EInventoryFrequency : uint8
{
	NOT_SET,
	Daily,
	Weekly
};

UENUM(BlueprintType)
enum class EInventoryOptionalField : uint8
{
	NOT_SET,
	Size,
	LastModifiedDate,
	StorageClass,
	ETag,
	IsMultipartUploaded,
	ReplicationStatus,
	EncryptionStatus,
	ObjectLockRetainUntilDate,
	ObjectLockMode,
	ObjectLockLegalHoldStatus,
	IntelligentTieringAccessTier
};

UENUM(BlueprintType)
enum class EInventoryIncludedObjectVersions : uint8
{
	NOT_SET,
	All,
	Current
};

UENUM(BlueprintType)
enum class EInventoryFormat : uint8
{
	NOT_SET,
	CSV,
	ORC,
	Parquet
};

UENUM(BlueprintType)
enum class EAnalyticsS3ExportFileFormat : uint8
{
	NOT_SET,
	CSV
};

UENUM(BlueprintType)
enum class EStorageClassAnalysisSchemaVersion : uint8
{
	NOT_SET,
	V_1
};

UENUM(BlueprintType)
enum class EPermission : uint8
{
	NOT_SET,
	FULL_CONTROL,
	WRITE,
	WRITE_ACP,
	READ,
	READ_ACP
};

UENUM(BlueprintType)
enum class EGranteeType : uint8
{
	NOT_SET,
	CanonicalUser,
	AmazonCustomerByEmail,
	Group
};

UENUM(BlueprintType)
enum class EBucketAccelerateStatus : uint8
{
	NOT_SET,
	Enabled,
	Suspended
};

UENUM(BlueprintType)
enum class EEncodingType : uint8
{
	NOT_SET,
	url
};

UENUM(BlueprintType)
enum class EBucketLocationConstraint : uint8
{
	NOT_SET,
	af_south_1,
	ap_east_1,
	ap_northeast_1,
	ap_northeast_2,
	ap_northeast_3,
	ap_south_1,
	ap_southeast_1,
	ap_southeast_2,
	ca_central_1,
	cn_north_1,
	cn_northwest_1,
	EU,
	eu_central_1,
	eu_north_1,
	eu_south_1,
	eu_west_1,
	eu_west_2,
	eu_west_3,
	me_south_1,
	sa_east_1,
	us_east_2,
	us_gov_east_1,
	us_gov_west_1,
	us_west_1,
	us_west_2,
	us_east_1
};

UENUM(BlueprintType)
enum class EBucketCannedACL : uint8
{
	NOT_SET,
	private_,
	public_read,
	public_read_write,
	authenticated_read
};

UENUM(BlueprintType)
enum class EObjectLockLegalHoldStatus : uint8
{
	NOT_SET,
	ON,
	OFF
};

UENUM(BlueprintType)
enum class EObjectLockMode : uint8
{
	NOT_SET,
	GOVERNANCE,
	COMPLIANCE
};

UENUM(BlueprintType)
enum class EServerSideEncryption : uint8
{
	NOT_SET,
	AES256,
	aws_kms
};

UENUM(BlueprintType)
enum class ETaggingDirective : uint8
{
	NOT_SET,
	COPY,
	REPLACE
};

UENUM(BlueprintType)
enum class EMetadataDirective : uint8
{
	NOT_SET,
	COPY,
	REPLACE
};

UENUM(BlueprintType)
enum class EObjectCannedACL : uint8
{
	NOT_SET,
	private_,
	public_read,
	public_read_write,
	authenticated_read,
	aws_exec_read,
	bucket_owner_read,
	bucket_owner_full_control
};

UENUM(BlueprintType)
enum class ERequestPayer : uint8
{
	NOT_SET,
	requester
};

UENUM(BlueprintType)
enum class ES3Scheme : uint8
{
	HTTP,
	HTTPS
};

UENUM(BlueprintType)
enum class ES3FollowRedirectsPolicy : uint8
{
	DEFAULT,
	ALWAYS,
	NEVER
};

UENUM(BlueprintType)
enum class ES3Errors : uint8
{
	//From Core//
	//////////////////////////////////////////////////////////////////////////////////////////
	INCOMPLETE_SIGNATURE = 0,
	INTERNAL_FAILURE = 1,
	INVALID_ACTION = 2,
	INVALID_CLIENT_TOKEN_ID = 3,
	INVALID_PARAMETER_COMBINATION = 4,
	INVALID_QUERY_PARAMETER = 5,
	INVALID_PARAMETER_VALUE = 6,
	MISSING_ACTION = 7,
	// SDK should never allow
	MISSING_AUTHENTICATION_TOKEN = 8,
	// SDK should never allow
	MISSING_PARAMETER = 9,
	// SDK should never allow
	OPT_IN_REQUIRED = 10,
	REQUEST_EXPIRED = 11,
	SERVICE_UNAVAILABLE = 12,
	THROTTLING = 13,
	VALIDATION = 14,
	ACCESS_DENIED = 15,
	RESOURCE_NOT_FOUND = 16,
	UNRECOGNIZED_CLIENT = 17,
	MALFORMED_QUERY_STRING = 18,
	SLOW_DOWN = 19,
	REQUEST_TIME_TOO_SKEWED = 20,
	INVALID_SIGNATURE = 21,
	SIGNATURE_DOES_NOT_MATCH = 22,
	INVALID_ACCESS_KEY_ID = 23,
	REQUEST_TIMEOUT = 24,
	NETWORK_CONNECTION = 99,
	UNKNOWN = 100,
	///////////////////////////////////////////////////////////////////////////////////////////

#if !DISABLE_S3
	BUCKET_ALREADY_EXISTS = static_cast<int>(Aws::Client::CoreErrors::SERVICE_EXTENSION_START_RANGE) + 1,
#endif
	BUCKET_ALREADY_OWNED_BY_YOU,
	NO_SUCH_BUCKET,
	NO_SUCH_KEY,
	NO_SUCH_UPLOAD,
	OBJECT_ALREADY_IN_ACTIVE_TIER,
	OBJECT_NOT_IN_ACTIVE_TIER
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CREDENTIALS AND CONFIG
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FAwsS3Credentials
{
	GENERATED_BODY()

public:
	FAwsS3Credentials()
		: AccessKeyId(FString())
		, SecretKey(FString())
		, SessionToken(FString())
		, Expiration(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString AccessKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString SecretKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString SessionToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FDateTime Expiration;
};

USTRUCT(BlueprintType)
struct FAwsS3ClientConfiguration
{
	GENERATED_BODY()

public:
	FAwsS3ClientConfiguration()
		: Sceheme(ES3Scheme::HTTPS)
		, Region("eu-central-1")
		, bUseDualStack(false)
		, MaxConnections(25)
		, HTTP_RequestTimeoutMs(0)
		, RequestTimeoutMs(3000)
		, ConnectTimeoutMs(1000)
		, bEnableTcpKeepAlive(true)
		, TcpKeepAliveIntervalMs(30)
		, LowSpeedLimit(1)
		, ProxyScheme(ES3Scheme::HTTP)
		, ProxyPort(0)
		, bVerifySSL(false)
		, FollowRedirects(ES3FollowRedirectsPolicy::DEFAULT)
		, bDisableExpectHeader(false)
		, bEnableClockSkewAdjustment(true)
		, bEnableHostPrefixInjection(true)
		, bEnableEndpointDiscovery(false)
	{
	}

public:
	// User Agent string user for http calls. This is filled in for you in the constructor. Don't override this unless you have a really good reason.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString UserAgent;
	// Http scheme to use. E.g. Http or Https. Default HTTPS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	ES3Scheme Sceheme;
	// AWS Region to use in signing requests. Default eu-west-2
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString Region;
	// Use dual stack endpoint32 in the endpoint32 calculation. It is your responsibility to verify that the service supports ipv6 in the region you select.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	bool bUseDualStack;
	// Max concurrent tcp connections for a single http client to use. Default 25.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	int32 MaxConnections;
	/**
	 * This is currently only applicable for Curl to set the http request level timeout, including possible dns lookup time, connection establish time, ssl handshake time and actual data transmission time.
	 * the corresponding Curl option is CURLOPT_TIMEOUT_MS
	 * defaults to 0, no http request level timeout.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	int32 HTTP_RequestTimeoutMs;
	/**
	 * Socket read timeouts for HTTP clients on Windows. Default 3000 ms. This should be more than adequate for most services. However, if you are transfering large amounts of data
	 * or are worried about higher latencies, you should set to something that makes more sense for your use case.
	 * For Curl, it's the low speed time, which contains the time in number milliseconds that transfer speed should be below "lowSpeedLimit" for the library to consider it too slow and abort.
	 * Note that for Curl this config is converted to seconds by rounding down to the nearest whole second except when the value is greater than 0 and less than 1000. In this case it is set to one second. When it's 0, low speed limit check will be disabled.
	 * Note that for Windows when this config is 0, the behavior is not specified by Windows.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	int32 RequestTimeoutMs;
	/**
	 * Socket connect timeout. Default 1000 ms. Unless you are very far away from your the data center you are talking to. 1000ms is more than sufficient.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	int32 ConnectTimeoutMs;
	/**
	 * Enable TCP keep-alive. Default true;
	 * No-op for WinHTTP, WinINet and IXMLHTTPRequest2 client.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	bool bEnableTcpKeepAlive;
	/**
	 * Interval to send a keep-alive packet over the connection. Default 30 seconds. Minimum 15 seconds.
	 * WinHTTP & libcurl support this option. Note that for Curl, this value will be rounded to an integer with second granularity.
	 * No-op for WinINet and IXMLHTTPRequest2 client.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	int32 TcpKeepAliveIntervalMs;
	/**
	 * Average transfer speed in bytes per second that the transfer should be below during the request timeout interval for it to be considered too slow and abort.
	 * Default 1 byte/second. Only for CURL client currently.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	int32 LowSpeedLimit;
	/**
	 * Override the http endpoint32 used to talk to a service.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString EndPointOverride;
	/**
	 * If you have users going through a proxy, set the proxy scheme here. Default HTTP
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	ES3Scheme ProxyScheme;
	/**
	 * If you have users going through a proxy, set the host here.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProxyHost;
	/**
	 * If you have users going through a proxy, set the port here.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	int32 ProxyPort;
	/**
	 * If you have users going through a proxy, set the username here.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProxyUsername;
	/**
	* If you have users going through a proxy, set the password here.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProxyPassword;
	/**
	* SSL Certificate file to use for connecting to an HTTPS proxy.
	* Used to set CURLOPT_PROXY_SSLCERT in libcurl. Example: client.pem
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProxySSL_CertPath;
	/**
	* Type of proxy client SSL certificate.
	* Used to set CURLOPT_PROXY_SSLCERTTYPE in libcurl. Example: PEM
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProxySSL_CertType;
	/**
	* Private key file to use for connecting to an HTTPS proxy.
	* Used to set CURLOPT_PROXY_SSLKEY in libcurl. Example: key.pem
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProxySSL_KeyPath;
	/**
	* Type of private key file used to connect to an HTTPS proxy.
	* Used to set CURLOPT_PROXY_SSLKEYTYPE in libcurl. Example: PEM
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProxySSL_KeyType;
	/**
	* Passphrase to the private key file used to connect to an HTTPS proxy.
	* Used to set CURLOPT_PROXY_KEYPASSWD in libcurl. Example: password1
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProxySSL_KeyPassword;
	/**
	 * If you need to test and want to get around TLS validation errors, do that here.
	 * you probably shouldn't use this flag in a production scenario.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	bool bVerifySSL;
	/**
	 * If your Certificate Authority path is different from the default, you can tell
	 * clients that aren't using the default trust store where to find your CA trust store.
	 * If you are on windows or apple, you likely don't want this.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString CaPath;
	/**
	 * If you certificate file is different from the default, you can tell clients that
	 * aren't using the default trust store where to find your ca file.
	 * If you are on windows or apple, you likely don't want this.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString CaFile;
	/**
	 * Sets the behavior how http stack handles 30x redirect codes.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	ES3FollowRedirectsPolicy FollowRedirects;
	/**
	 * Only works for Curl http client.
	 * Curl will by default add "Expect: 100-Continue" header in a Http request so as to avoid sending http
	 * payload to wire if server respond error immediately after receiving the header.
	 * Set this option to true will tell Curl to send http request header and body together.
	 * This can save one round-trip time and especially useful when the payload is small and network latency is more important.
	 * But be careful when Http request has large payload such S3 PutObject. You don't want to spend long time sending a large payload just getting a error response for server.
	 * The default value will be false.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	bool bDisableExpectHeader;
	/**
	 * If set to true clock skew will be adjusted after each http attempt, default to true.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	bool bEnableClockSkewAdjustment;

	/**
	 * Enable host prefix injection.
	 * For services whose endpoint32 is injectable. e.g. servicediscovery, you can modify the http host's prefix so as to add "data-" prefix for DiscoverInstances request.
	 * Default to true, enabled. You can disable it for testing purpose.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	bool bEnableHostPrefixInjection;

	/**
	 * Enable endpoint32 discovery
	 * For some services to dynamically set up their endpoints for different requests.
	 * Defaults to false, it's an opt-in feature.
	 * If disabled, regional or overriden endpoint32 will be used instead.
	 * If a request requires endpoint32 discovery but you disabled it. The request will never succeed.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	bool bEnableEndpointDiscovery;

	/**
	 * profileName in config file that will be used by this object to reslove more configurations.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|Lambda")
	FString ProfileName;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FChecksum
{
	GENERATED_BODY()

public:
	FChecksum() = default;
#if !DISABLE_S3
	FChecksum(const Aws::S3::Model::Checksum& Data)
		: ChecksumCRC32(UTF8_TO_TCHAR(Data.GetChecksumCRC32().c_str()))
		, ChecksumCRC32C(UTF8_TO_TCHAR(Data.GetChecksumCRC32C().c_str()))
		, ChecksumSHA1(UTF8_TO_TCHAR(Data.GetChecksumSHA1().c_str()))
		, ChecksumSHA256(UTF8_TO_TCHAR(Data.GetChecksumSHA256().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ChecksumCRC32;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ChecksumCRC32C;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ChecksumSHA1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ChecksumSHA256;
};

USTRUCT(BlueprintType)
struct FS3Error
{
	GENERATED_BODY()

public:
	FS3Error() = default;
#if !DISABLE_S3
	FS3Error(const Aws::S3::Model::Error& Data)
		: Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, Code(UTF8_TO_TCHAR(Data.GetCode().c_str()))
		, Message(UTF8_TO_TCHAR(Data.GetMessage().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Code;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Message;
};

USTRUCT(BlueprintType)
struct FPublicAccessBlockConfiguration
{
	GENERATED_BODY()

public:
	FPublicAccessBlockConfiguration()
		: bBlockPublicAcls(false)
		, bIgnorePublicAcls(false)
		, bBlockPublicPolicy(false)
		, bRestrictPublicBuckets(false)
	{
	}
#if !DISABLE_S3
	FPublicAccessBlockConfiguration(const Aws::S3::Model::PublicAccessBlockConfiguration& Data)
		: bBlockPublicAcls(Data.GetBlockPublicAcls())
		, bIgnorePublicAcls(Data.GetIgnorePublicAcls())
		, bBlockPublicPolicy(Data.GetBlockPublicPolicy())
		, bRestrictPublicBuckets(Data.GetRestrictPublicBuckets())
	{
	}

public:
	operator Aws::S3::Model::PublicAccessBlockConfiguration() const
	{
		Aws::S3::Model::PublicAccessBlockConfiguration Data = {};
		Data.SetBlockPublicAcls(bBlockPublicAcls);
		Data.SetIgnorePublicAcls(bIgnorePublicAcls);
		Data.SetBlockPublicPolicy(bBlockPublicPolicy);
		Data.SetRestrictPublicBuckets(bRestrictPublicBuckets);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bBlockPublicAcls;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIgnorePublicAcls;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bBlockPublicPolicy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bRestrictPublicBuckets;
};

USTRUCT(BlueprintType)
struct FObjectLockRetention
{
	GENERATED_BODY()

public:
	FObjectLockRetention()
		: Mode(EObjectLockRetentionMode::NOT_SET)
		, RetainUntilDate(0)
	{
	}
#if !DISABLE_S3
	FObjectLockRetention(const Aws::S3::Model::ObjectLockRetention& Data)
		: Mode(static_cast<EObjectLockRetentionMode>(Data.GetMode()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetRetainUntilDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), RetainUntilDate);
	}

public:
	operator Aws::S3::Model::ObjectLockRetention() const
	{
		Aws::S3::Model::ObjectLockRetention Data = {};
		if (Mode != EObjectLockRetentionMode::NOT_SET)
			Data.SetMode(static_cast<Aws::S3::Model::ObjectLockRetentionMode>(Mode));
		if (RetainUntilDate > 0)
			Data.SetRetainUntilDate(Aws::Utils::DateTime(TCHAR_TO_UTF8(*RetainUntilDate.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockRetentionMode Mode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime RetainUntilDate;
};

USTRUCT(BlueprintType)
struct FPolicyStatus
{
	GENERATED_BODY()

public:
	FPolicyStatus()
		: bIsPublic(false)
	{
	}
#if !DISABLE_S3
	FPolicyStatus(const Aws::S3::Model::PolicyStatus& Data)
		: bIsPublic(Data.GetIsPublic())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsPublic;
};

USTRUCT(BlueprintType)
struct FObjectLockLegalHold
{
	GENERATED_BODY()

public:
	FObjectLockLegalHold()
		: Status(EObjectLockLegalHoldStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FObjectLockLegalHold(const Aws::S3::Model::ObjectLockLegalHold& Data)
		: Status(static_cast<EObjectLockLegalHoldStatus>(Data.GetStatus()))
	{
	}

public:
	operator Aws::S3::Model::ObjectLockLegalHold() const
	{
		Aws::S3::Model::ObjectLockLegalHold Data = {};
		if (Status != EObjectLockLegalHoldStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::ObjectLockLegalHoldStatus>(Status));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockLegalHoldStatus Status;
};

USTRUCT(BlueprintType)
struct FBucket
{
	GENERATED_BODY()

public:
	FBucket()
		: CreationDate(0)
	{
	}
#if !DISABLE_S3
	FBucket(const Aws::S3::Model::Bucket& Data)
		: Name(UTF8_TO_TCHAR(Data.GetName().c_str()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetCreationDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), CreationDate);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime CreationDate;
};

USTRUCT(BlueprintType)
struct FDefaultRetention
{
	GENERATED_BODY()

public:
	FDefaultRetention()
		: Mode(EObjectLockRetentionMode::NOT_SET)
		, Days(0)
		, Years(0)
	{
	}
#if !DISABLE_S3
	FDefaultRetention(const Aws::S3::Model::DefaultRetention& Data)
		: Mode(static_cast<EObjectLockRetentionMode>(Data.GetMode()))
		, Days(Data.GetDays())
		, Years(Data.GetYears())
	{
	}

public:
	operator Aws::S3::Model::DefaultRetention() const
	{
		Aws::S3::Model::DefaultRetention Data = {};
		if (Mode != EObjectLockRetentionMode::NOT_SET)
			Data.SetMode(static_cast<Aws::S3::Model::ObjectLockRetentionMode>(Mode));
		if (Days > 0)
			Data.SetDays(Days);
		if (Years > 0)
			Data.SetYears(Years);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockRetentionMode Mode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 Days;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 Years;
};

USTRUCT(BlueprintType)
struct FObjectLockRule
{
	GENERATED_BODY()

public:
	FObjectLockRule() = default;
#if !DISABLE_S3
	FObjectLockRule(const Aws::S3::Model::ObjectLockRule& Data)
		: DefaultRetention(Data.GetDefaultRetention())
	{
	}

public:
	operator Aws::S3::Model::ObjectLockRule() const
	{
		Aws::S3::Model::ObjectLockRule Data = {};
		Data.SetDefaultRetention(DefaultRetention);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDefaultRetention DefaultRetention;
};

USTRUCT(BlueprintType)
struct FObjectLockConfiguration
{
	GENERATED_BODY()

public:
	FObjectLockConfiguration()
		: ObjectLockEnabled(EObjectLockEnabled::NOT_SET)
	{
	}
#if !DISABLE_S3
	FObjectLockConfiguration(const Aws::S3::Model::ObjectLockConfiguration& Data)
		: ObjectLockEnabled(static_cast<EObjectLockEnabled>(Data.GetObjectLockEnabled()))
		, Rule(Data.GetRule())
	{
	}

public:
	operator Aws::S3::Model::ObjectLockConfiguration() const
	{
		Aws::S3::Model::ObjectLockConfiguration Data = {};
		if (ObjectLockEnabled != EObjectLockEnabled::NOT_SET)
			Data.SetObjectLockEnabled(static_cast<Aws::S3::Model::ObjectLockEnabled>(ObjectLockEnabled));
		Data.SetRule(Rule);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockEnabled ObjectLockEnabled;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FObjectLockRule Rule;
};

USTRUCT(BlueprintType)
struct FS3Tag
{
	GENERATED_BODY()

public:
	FS3Tag() = default;
#if !DISABLE_S3
	FS3Tag(const Aws::S3::Model::Tag& Data)
		: Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, Value(UTF8_TO_TCHAR(Data.GetValue().c_str()))
	{
	}

public:
	operator Aws::S3::Model::Tag() const
	{
		Aws::S3::Model::Tag Data = {};
		if (Key.IsEmpty() == false)
			Data.SetKey(TCHAR_TO_UTF8(*Key));
		if (Value.IsEmpty() == false)
			Data.SetValue(TCHAR_TO_UTF8(*Value));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Value;
};

USTRUCT(BlueprintType)
struct FGrantee
{
	GENERATED_BODY()

public:
	FGrantee()
		: Type(EGranteeType::NOT_SET)
	{
	}
#if !DISABLE_S3
	FGrantee(const Aws::S3::Model::Grantee& Data)
		: DisplayName(UTF8_TO_TCHAR(Data.GetDisplayName().c_str()))
		, EmailAddress(UTF8_TO_TCHAR(Data.GetEmailAddress().c_str()))
		, ID(UTF8_TO_TCHAR(Data.GetID().c_str()))
		, Type(static_cast<EGranteeType>(Data.GetType()))
		, URI(UTF8_TO_TCHAR(Data.GetURI().c_str()))
	{
	}

public:
	operator Aws::S3::Model::Grantee() const
	{
		Aws::S3::Model::Grantee Data = {};
		if (DisplayName.IsEmpty() == false)
			Data.SetDisplayName(TCHAR_TO_UTF8(*DisplayName));
		if (EmailAddress.IsEmpty() == false)
			Data.SetEmailAddress(TCHAR_TO_UTF8(*EmailAddress));
		if (ID.IsEmpty() == false)
			Data.SetID(TCHAR_TO_UTF8(*ID));
		if (Type != EGranteeType::NOT_SET)
			Data.SetType(static_cast<Aws::S3::Model::Type>(Type));
		if (URI.IsEmpty() == false)
			Data.SetURI(TCHAR_TO_UTF8(*URI));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString DisplayName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString EmailAddress;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EGranteeType Type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString URI;
};

USTRUCT(BlueprintType)
struct FErrorDocument
{
	GENERATED_BODY()

public:
	FErrorDocument() = default;
#if !DISABLE_S3
	FErrorDocument(const Aws::S3::Model::ErrorDocument& Data)
		: Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
	{
	}

public:
	operator Aws::S3::Model::ErrorDocument() const
	{
		Aws::S3::Model::ErrorDocument Data = {};
		if (Key.IsEmpty() == false)
			Data.SetKey(TCHAR_TO_UTF8(*Key));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
};

USTRUCT(BlueprintType)
struct FIndexDocument
{
	GENERATED_BODY()

public:
	FIndexDocument() = default;
#if !DISABLE_S3
	FIndexDocument(const Aws::S3::Model::IndexDocument& Data)
		: Suffix(UTF8_TO_TCHAR(Data.GetSuffix().c_str()))
	{
	}

public:
	operator Aws::S3::Model::IndexDocument() const
	{
		Aws::S3::Model::IndexDocument Data = {};
		if (Suffix.IsEmpty() == false)
			Data.SetSuffix(TCHAR_TO_UTF8(*Suffix));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Suffix;
};

USTRUCT(BlueprintType)
struct FRedirectAllRequestsTo
{
	GENERATED_BODY()

public:
	FRedirectAllRequestsTo()
		: Protocol(ES3Protocol::NOT_SET)
	{
	}
#if !DISABLE_S3
	FRedirectAllRequestsTo(const Aws::S3::Model::RedirectAllRequestsTo& Data)
		: HostName(UTF8_TO_TCHAR(Data.GetHostName().c_str()))
		, Protocol(static_cast<ES3Protocol>(Data.GetProtocol()))
	{
	}

public:
	operator Aws::S3::Model::RedirectAllRequestsTo() const
	{
		Aws::S3::Model::RedirectAllRequestsTo Data = {};
		if (HostName.IsEmpty() == false)
			Data.SetHostName(TCHAR_TO_UTF8(*HostName));
		if (Protocol != ES3Protocol::NOT_SET)
			Data.SetProtocol(static_cast<Aws::S3::Model::Protocol>(Protocol));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString HostName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ES3Protocol Protocol;
};

USTRUCT(BlueprintType)
struct FRedirect
{
	GENERATED_BODY()

public:
	FRedirect()
		: Protocol(ES3Protocol::NOT_SET)
	{
	}
#if !DISABLE_S3
	FRedirect(const Aws::S3::Model::Redirect& Data)
		: HostName(UTF8_TO_TCHAR(Data.GetHostName().c_str()))
		, HttpRedirectCode(UTF8_TO_TCHAR(Data.GetHttpRedirectCode().c_str()))
		, Protocol(static_cast<ES3Protocol>(Data.GetProtocol()))
		, ReplaceKeyPrefixWith(UTF8_TO_TCHAR(Data.GetReplaceKeyPrefixWith().c_str()))
		, ReplaceKeyWith(UTF8_TO_TCHAR(Data.GetReplaceKeyWith().c_str()))
	{
	}

public:
	operator Aws::S3::Model::Redirect() const
	{
		Aws::S3::Model::Redirect Data = {};
		if (HostName.IsEmpty() == false)
			Data.SetHostName(TCHAR_TO_UTF8(*HostName));
		if (HttpRedirectCode.IsEmpty() == false)
			Data.SetHttpRedirectCode(TCHAR_TO_UTF8(*HttpRedirectCode));
		if (Protocol != ES3Protocol::NOT_SET)
			Data.SetProtocol(static_cast<Aws::S3::Model::Protocol>(Protocol));
		if (ReplaceKeyPrefixWith.IsEmpty() == false)
			Data.SetReplaceKeyPrefixWith(TCHAR_TO_UTF8(*ReplaceKeyPrefixWith));
		if (ReplaceKeyWith.IsEmpty() == false)
			Data.SetReplaceKeyWith(TCHAR_TO_UTF8(*ReplaceKeyWith));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString HostName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString HttpRedirectCode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ES3Protocol Protocol;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ReplaceKeyPrefixWith;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ReplaceKeyWith;
};

USTRUCT(BlueprintType)
struct FS3AttributeValue
{
	GENERATED_BODY()

public:
	FS3AttributeValue() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString S;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString N;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FString> SL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> SDM;
};

USTRUCT(BlueprintType)
struct FS3Condition
{
	GENERATED_BODY()

public:
	FS3Condition() = default;
#if !DISABLE_S3
	FS3Condition(const Aws::S3::Model::Condition& Data)
		: HttpErrorCodeReturnedEquals(UTF8_TO_TCHAR(Data.GetHttpErrorCodeReturnedEquals().c_str()))
		, KeyPrefixEquals(UTF8_TO_TCHAR(Data.GetKeyPrefixEquals().c_str()))
	{
	}

public:
	operator Aws::S3::Model::Condition() const
	{
		Aws::S3::Model::Condition Data = {};
		if (HttpErrorCodeReturnedEquals.IsEmpty() == false)
			Data.SetHttpErrorCodeReturnedEquals(TCHAR_TO_UTF8(*HttpErrorCodeReturnedEquals));
		if (KeyPrefixEquals.IsEmpty() == false)
			Data.SetKeyPrefixEquals(TCHAR_TO_UTF8(*KeyPrefixEquals));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString HttpErrorCodeReturnedEquals;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KeyPrefixEquals;
};

USTRUCT(BlueprintType)
struct FRoutingRule
{
	GENERATED_BODY()

public:
	FRoutingRule() = default;
#if !DISABLE_S3
	FRoutingRule(const Aws::S3::Model::RoutingRule& Data)
		: Condition(Data.GetCondition())
		, Redirect(Data.GetRedirect())
	{
	}

public:
	operator Aws::S3::Model::RoutingRule() const
	{
		Aws::S3::Model::RoutingRule Data = {};
		Data.SetCondition(Condition);
		Data.SetRedirect(Redirect);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Condition Condition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FRedirect Redirect;
};

USTRUCT(BlueprintType)
struct FWebsiteConfiguration
{
	GENERATED_BODY()

public:
	FWebsiteConfiguration() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::WebsiteConfiguration() const
	{
		Aws::S3::Model::WebsiteConfiguration Data = {};
		Data.SetErrorDocument(ErrorDocument);
		Data.SetIndexDocument(IndexDocument);
		Data.SetRedirectAllRequestsTo(RedirectAllRequestsTo);
		for (auto& Element : RoutingRules)
			Data.AddRoutingRules(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FErrorDocument ErrorDocument;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FIndexDocument IndexDocument;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FRedirectAllRequestsTo RedirectAllRequestsTo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FRoutingRule> RoutingRules;
};

USTRUCT(BlueprintType)
struct FVersioningConfiguration
{
	GENERATED_BODY()

public:
	FVersioningConfiguration()
		: MFADelete(EMFADelete::NOT_SET)
		, Status(EBucketVersioningStatus::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::VersioningConfiguration() const
	{
		Aws::S3::Model::VersioningConfiguration Data = {};
		if (MFADelete != EMFADelete::NOT_SET)
			Data.SetMFADelete(static_cast<Aws::S3::Model::MFADelete>(MFADelete));
		if (Status != EBucketVersioningStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::BucketVersioningStatus>(Status));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EMFADelete MFADelete;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketVersioningStatus Status;
};

USTRUCT(BlueprintType)
struct FS3Tagging
{
	GENERATED_BODY()

public:
	FS3Tagging() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::Tagging() const
	{
		Aws::S3::Model::Tagging Data = {};
		for (auto& Element : TagSet)
			Data.AddTagSet(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Tag> TagSet;
};

USTRUCT(BlueprintType)
struct FRequestPaymentConfiguration
{
	GENERATED_BODY()

public:
	FRequestPaymentConfiguration()
		: Payer(EPayer::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::RequestPaymentConfiguration() const
	{
		Aws::S3::Model::RequestPaymentConfiguration Data = {};
		if (Payer != EPayer::NOT_SET)
			Data.SetPayer(static_cast<Aws::S3::Model::Payer>(Payer));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EPayer Payer;
};

USTRUCT(BlueprintType)
struct FReplicationRuleAndOperator
{
	GENERATED_BODY()

public:
	FReplicationRuleAndOperator() = default;
#if !DISABLE_S3
	FReplicationRuleAndOperator(const Aws::S3::Model::ReplicationRuleAndOperator& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
	{
		for (auto& Element : Data.GetTags())
			Tags.Add(Element);
	}

public:
	operator Aws::S3::Model::ReplicationRuleAndOperator() const
	{
		Aws::S3::Model::ReplicationRuleAndOperator Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		for (auto& Element : Tags)
			Data.AddTags(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Tag> Tags;
};

USTRUCT(BlueprintType)
struct FReplicationRuleFilter
{
	GENERATED_BODY()

public:
	FReplicationRuleFilter() = default;
#if !DISABLE_S3
	FReplicationRuleFilter(const Aws::S3::Model::ReplicationRuleFilter& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Tag(Data.GetTag())
		, And(Data.GetAnd())
	{
	}

public:
	operator Aws::S3::Model::ReplicationRuleFilter() const
	{
		Aws::S3::Model::ReplicationRuleFilter Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		Data.SetTag(Tag);
		Data.SetAnd(And);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Tag Tag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FReplicationRuleAndOperator And;
};

USTRUCT(BlueprintType)
struct FSseKmsEncryptedObjects
{
	GENERATED_BODY()

public:
	FSseKmsEncryptedObjects()
		: Status(ESseKmsEncryptedObjectsStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FSseKmsEncryptedObjects(const Aws::S3::Model::SseKmsEncryptedObjects& Data)
		: Status(static_cast<ESseKmsEncryptedObjectsStatus>(Data.GetStatus()))
	{
	}

public:
	operator Aws::S3::Model::SseKmsEncryptedObjects() const
	{
		Aws::S3::Model::SseKmsEncryptedObjects Data = {};
		if (Status != ESseKmsEncryptedObjectsStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::SseKmsEncryptedObjectsStatus>(Status));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ESseKmsEncryptedObjectsStatus Status;
};

USTRUCT(BlueprintType)
struct FSourceSelectionCriteria
{
	GENERATED_BODY()

public:
	FSourceSelectionCriteria() = default;
#if !DISABLE_S3
	FSourceSelectionCriteria(const Aws::S3::Model::SourceSelectionCriteria& Data)
		: SseKmsEncryptedObjects(Data.GetSseKmsEncryptedObjects())
	{
	}

public:
	operator Aws::S3::Model::SourceSelectionCriteria() const
	{
		Aws::S3::Model::SourceSelectionCriteria Data = {};
		Data.SetSseKmsEncryptedObjects(SseKmsEncryptedObjects);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FSseKmsEncryptedObjects SseKmsEncryptedObjects;
};

USTRUCT(BlueprintType)
struct FExistingObjectReplication
{
	GENERATED_BODY()

public:
	FExistingObjectReplication()
		: Status(EExistingObjectReplicationStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FExistingObjectReplication(const Aws::S3::Model::ExistingObjectReplication& Data)
		: Status(static_cast<EExistingObjectReplicationStatus>(Data.GetStatus()))
	{
	}

public:
	operator Aws::S3::Model::ExistingObjectReplication() const
	{
		Aws::S3::Model::ExistingObjectReplication Data = {};
		if (Status != EExistingObjectReplicationStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::ExistingObjectReplicationStatus>(Status));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EExistingObjectReplicationStatus Status;
};

USTRUCT(BlueprintType)
struct FAccessControlTranslation
{
	GENERATED_BODY()

public:
	FAccessControlTranslation()
		: Owner(EOwnerOverride::NOT_SET)
	{
	}
#if !DISABLE_S3
	FAccessControlTranslation(const Aws::S3::Model::AccessControlTranslation& Data)
		: Owner(static_cast<EOwnerOverride>(Data.GetOwner()))
	{
	}

public:
	operator Aws::S3::Model::AccessControlTranslation() const
	{
		Aws::S3::Model::AccessControlTranslation Data = {};
		if (Owner != EOwnerOverride::NOT_SET)
			Data.SetOwner(static_cast<Aws::S3::Model::OwnerOverride>(Owner));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EOwnerOverride Owner;
};

USTRUCT(BlueprintType)
struct FEncryptionConfiguration
{
	GENERATED_BODY()

public:
	FEncryptionConfiguration() = default;
#if !DISABLE_S3
	FEncryptionConfiguration(const Aws::S3::Model::EncryptionConfiguration& Data)
		: ReplicaKmsKeyID(UTF8_TO_TCHAR(Data.GetReplicaKmsKeyID().c_str()))
	{
	}

public:
	operator Aws::S3::Model::EncryptionConfiguration() const
	{
		Aws::S3::Model::EncryptionConfiguration Data = {};
		if (ReplicaKmsKeyID.IsEmpty() == false)
			Data.SetReplicaKmsKeyID(TCHAR_TO_UTF8(*ReplicaKmsKeyID));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ReplicaKmsKeyID;
};

USTRUCT(BlueprintType)
struct FReplicationTimeValue
{
	GENERATED_BODY()

public:
	FReplicationTimeValue()
		: Minutes(0)
	{
	}
#if !DISABLE_S3
	FReplicationTimeValue(const Aws::S3::Model::ReplicationTimeValue& Data)
		: Minutes(Data.GetMinutes())
	{
	}

public:
	operator Aws::S3::Model::ReplicationTimeValue() const
	{
		Aws::S3::Model::ReplicationTimeValue Data = {};
		if (Minutes > 0)
			Data.SetMinutes(Minutes);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 Minutes;
};

USTRUCT(BlueprintType)
struct FReplicationTime
{
	GENERATED_BODY()

public:
	FReplicationTime()
		: Status(EReplicationTimeStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FReplicationTime(const Aws::S3::Model::ReplicationTime& Data)
		: Status(static_cast<EReplicationTimeStatus>(Data.GetStatus()))
		, Time(Data.GetTime())
	{
	}

public:
	operator Aws::S3::Model::ReplicationTime() const
	{
		Aws::S3::Model::ReplicationTime Data = {};
		if (Status != EReplicationTimeStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::ReplicationTimeStatus>(Status));
		Data.SetTime(Time);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EReplicationTimeStatus Status;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FReplicationTimeValue Time;
};

USTRUCT(BlueprintType)
struct FMetrics
{
	GENERATED_BODY()

public:
	FMetrics()
		: Status(EMetricsStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FMetrics(const Aws::S3::Model::Metrics& Data)
		: Status(static_cast<EMetricsStatus>(Data.GetStatus()))
		, EventThreshold(Data.GetEventThreshold())
	{
	}

public:
	operator Aws::S3::Model::Metrics() const
	{
		Aws::S3::Model::Metrics Data = {};
		if (Status != EMetricsStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::MetricsStatus>(Status));
		Data.SetEventThreshold(EventThreshold);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EMetricsStatus Status;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FReplicationTimeValue EventThreshold;
};

USTRUCT(BlueprintType)
struct FDestination
{
	GENERATED_BODY()

public:
	FDestination()
		: StorageClass(EStorageClass::NOT_SET)
	{
	}
#if !DISABLE_S3
	FDestination(const Aws::S3::Model::Destination& Data)
		: Bucket(UTF8_TO_TCHAR(Data.GetBucket().c_str()))
		, Account(UTF8_TO_TCHAR(Data.GetAccount().c_str()))
		, StorageClass(static_cast<EStorageClass>(Data.GetStorageClass()))
		, AccessControlTranslation(Data.GetAccessControlTranslation())
		, EncryptionConfiguration(Data.GetEncryptionConfiguration())
		, ReplicationTime(Data.GetReplicationTime())
		, Metrics(Data.GetMetrics())
	{
	}

public:
	operator Aws::S3::Model::Destination() const
	{
		Aws::S3::Model::Destination Data = {};
		if (Bucket.IsEmpty() == false)
			Data.SetBucket(TCHAR_TO_UTF8(*Bucket));
		if (Account.IsEmpty() == false)
			Data.SetAccount(TCHAR_TO_UTF8(*Account));
		if (StorageClass != EStorageClass::NOT_SET)
			Data.SetStorageClass(static_cast<Aws::S3::Model::StorageClass>(StorageClass));
		Data.SetAccessControlTranslation(AccessControlTranslation);
		Data.SetEncryptionConfiguration(EncryptionConfiguration);
		Data.SetReplicationTime(ReplicationTime);
		Data.SetMetrics(Metrics);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Account;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAccessControlTranslation AccessControlTranslation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FEncryptionConfiguration EncryptionConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FReplicationTime ReplicationTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FMetrics Metrics;
};

USTRUCT(BlueprintType)
struct FDeleteMarkerReplication
{
	GENERATED_BODY()

public:
	FDeleteMarkerReplication()
		: Status(EDeleteMarkerReplicationStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FDeleteMarkerReplication(const Aws::S3::Model::DeleteMarkerReplication& Data)
		: Status(static_cast<EDeleteMarkerReplicationStatus>(Data.GetStatus()))
	{
	}

public:
	operator Aws::S3::Model::DeleteMarkerReplication() const
	{
		Aws::S3::Model::DeleteMarkerReplication Data = {};
		if (Status != EDeleteMarkerReplicationStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::DeleteMarkerReplicationStatus>(Status));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EDeleteMarkerReplicationStatus Status;
};

USTRUCT(BlueprintType)
struct FReplicationRule
{
	GENERATED_BODY()

public:
	FReplicationRule()
		: Priority(0)
		, Status(EReplicationRuleStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FReplicationRule(const Aws::S3::Model::ReplicationRule& Data)
		: ID(UTF8_TO_TCHAR(Data.GetID().c_str()))
		, Priority(Data.GetPriority())
		, Filter(Data.GetFilter())
		, Status(static_cast<EReplicationRuleStatus>(Data.GetStatus()))
		, SourceSelectionCriteria(Data.GetSourceSelectionCriteria())
		, ExistingObjectReplication(Data.GetExistingObjectReplication())
		, Destination(Data.GetDestination())
		, DeleteMarkerReplication(Data.GetDeleteMarkerReplication())
	{
	}

public:
	operator Aws::S3::Model::ReplicationRule() const
	{
		Aws::S3::Model::ReplicationRule Data = {};
		if (ID.IsEmpty() == false)
			Data.SetID(TCHAR_TO_UTF8(*ID));
		Data.SetPriority(Priority);
		Data.SetFilter(Filter);
		if (Status != EReplicationRuleStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::ReplicationRuleStatus>(Status));
		Data.SetSourceSelectionCriteria(SourceSelectionCriteria);
		Data.SetExistingObjectReplication(ExistingObjectReplication);
		Data.SetDestination(Destination);
		Data.SetDeleteMarkerReplication(DeleteMarkerReplication);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 Priority;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FReplicationRuleFilter Filter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EReplicationRuleStatus Status;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FSourceSelectionCriteria SourceSelectionCriteria;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FExistingObjectReplication ExistingObjectReplication;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDestination Destination;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDeleteMarkerReplication DeleteMarkerReplication;
};

USTRUCT(BlueprintType)
struct FReplicationConfiguration
{
	GENERATED_BODY()

public:
	FReplicationConfiguration() = default;
#if !DISABLE_S3
	FReplicationConfiguration(const Aws::S3::Model::ReplicationConfiguration& Data)
		: Role(UTF8_TO_TCHAR(Data.GetRole().c_str()))
	{
		for (auto& Element : Data.GetRules())
			Rules.Add(Element);
	}

public:
	operator Aws::S3::Model::ReplicationConfiguration() const
	{
		Aws::S3::Model::ReplicationConfiguration Data = {};
		if (Role.IsEmpty() == false)
			Data.SetRole(TCHAR_TO_UTF8(*Role));
		for (auto& Element : Rules)
			Data.AddRules(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Role;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FReplicationRule> Rules;
};

USTRUCT(BlueprintType)
struct FFilterRule
{
	GENERATED_BODY()

public:
	FFilterRule()
		: Name(EFilterRuleName::NOT_SET)
	{
	}
#if !DISABLE_S3
	FFilterRule(const Aws::S3::Model::FilterRule& Data)
		: Name(static_cast<EFilterRuleName>(Data.GetName()))
		, Value(UTF8_TO_TCHAR(Data.GetValue().c_str()))
	{
	}

public:
	operator Aws::S3::Model::FilterRule() const
	{
		Aws::S3::Model::FilterRule Data = {};
		if (Name != EFilterRuleName::NOT_SET)
			Data.SetName(static_cast<Aws::S3::Model::FilterRuleName>(Name));
		if (Value.IsEmpty() == false)
			Data.SetValue(TCHAR_TO_UTF8(*Value));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EFilterRuleName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Value;
};

USTRUCT(BlueprintType)
struct FS3KeyFilter
{
	GENERATED_BODY()

public:
	FS3KeyFilter() = default;
#if !DISABLE_S3
	FS3KeyFilter(const Aws::S3::Model::S3KeyFilter& Data)
	{
		for (auto& Element : Data.GetFilterRules())
			FilterRules.Add(Element);
	}

public:
	operator Aws::S3::Model::S3KeyFilter() const
	{
		Aws::S3::Model::S3KeyFilter Data = {};
		for (auto& Element : FilterRules)
			Data.AddFilterRules(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FFilterRule> FilterRules;
};

USTRUCT(BlueprintType)
struct FNotificationConfigurationFilter
{
	GENERATED_BODY()

public:
	FNotificationConfigurationFilter() = default;
#if !DISABLE_S3
	FNotificationConfigurationFilter(const Aws::S3::Model::NotificationConfigurationFilter& Data)
		: Key(Data.GetKey())
	{
	}

public:
	operator Aws::S3::Model::NotificationConfigurationFilter() const
	{
		Aws::S3::Model::NotificationConfigurationFilter Data = {};
		Data.SetKey(Key);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3KeyFilter Key;
};

USTRUCT(BlueprintType)
struct FTopicConfiguration
{
	GENERATED_BODY()

public:
	FTopicConfiguration() = default;
#if !DISABLE_S3
	FTopicConfiguration(const Aws::S3::Model::TopicConfiguration& Data)
		: Id(UTF8_TO_TCHAR(Data.GetId().c_str()))
		, TopicArn(UTF8_TO_TCHAR(Data.GetTopicArn().c_str()))
		, Filter(Data.GetFilter())
	{
		for (auto& Element : Data.GetEvents())
			Events.Add(static_cast<ES3Event>(Element));
	}

public:
	operator Aws::S3::Model::TopicConfiguration() const
	{
		Aws::S3::Model::TopicConfiguration Data = {};
		if (Id.IsEmpty() == false)
			Data.SetId(TCHAR_TO_UTF8(*Id));
		if (TopicArn.IsEmpty() == false)
			Data.SetTopicArn(TCHAR_TO_UTF8(*TopicArn));
		for (auto& Element : Events)
			Data.AddEvents(static_cast<Aws::S3::Model::Event>(Element));
		Data.SetFilter(Filter);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString TopicArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<ES3Event> Events;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FNotificationConfigurationFilter Filter;
};

USTRUCT(BlueprintType)
struct FQueueConfiguration
{
	GENERATED_BODY()

public:
	FQueueConfiguration() = default;
#if !DISABLE_S3
	FQueueConfiguration(const Aws::S3::Model::QueueConfiguration& Data)
		: Id(UTF8_TO_TCHAR(Data.GetId().c_str()))
		, QueueArn(UTF8_TO_TCHAR(Data.GetQueueArn().c_str()))
		, Filter(Data.GetFilter())
	{
		for (auto& Element : Data.GetEvents())
			Events.Add(static_cast<ES3Event>(Element));
	}

public:
	operator Aws::S3::Model::QueueConfiguration() const
	{
		Aws::S3::Model::QueueConfiguration Data = {};
		if (Id.IsEmpty() == false)
			Data.SetId(TCHAR_TO_UTF8(*Id));
		if (QueueArn.IsEmpty() == false)
			Data.SetQueueArn(TCHAR_TO_UTF8(*QueueArn));
		for (auto& Element : Events)
			Data.AddEvents(static_cast<Aws::S3::Model::Event>(Element));
		Data.SetFilter(Filter);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString QueueArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<ES3Event> Events;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FNotificationConfigurationFilter Filter;
};

USTRUCT(BlueprintType)
struct FLambdaFunctionConfiguration
{
	GENERATED_BODY()

public:
	FLambdaFunctionConfiguration() = default;
#if !DISABLE_S3
	FLambdaFunctionConfiguration(const Aws::S3::Model::LambdaFunctionConfiguration& Data)
		: Id(UTF8_TO_TCHAR(Data.GetId().c_str()))
		, LambdaFunctionArn(UTF8_TO_TCHAR(Data.GetLambdaFunctionArn().c_str()))
		, Filter(Data.GetFilter())
	{
		for (auto& Element : Data.GetEvents())
			Events.Add(static_cast<ES3Event>(Element));
	}

public:
	operator Aws::S3::Model::LambdaFunctionConfiguration() const
	{
		Aws::S3::Model::LambdaFunctionConfiguration Data = {};
		if (Id.IsEmpty() == false)
			Data.SetId(TCHAR_TO_UTF8(*Id));
		if (LambdaFunctionArn.IsEmpty() == false)
			Data.SetLambdaFunctionArn(TCHAR_TO_UTF8(*LambdaFunctionArn));
		for (auto& Element : Events)
			Data.AddEvents(static_cast<Aws::S3::Model::Event>(Element));
		Data.SetFilter(Filter);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString LambdaFunctionArn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<ES3Event> Events;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FNotificationConfigurationFilter Filter;
};

USTRUCT(BlueprintType)
struct FNotificationConfiguration
{
	GENERATED_BODY()

public:
	FNotificationConfiguration() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::NotificationConfiguration() const
	{
		Aws::S3::Model::NotificationConfiguration Data = {};
		for (auto& Element : TopicConfigurations)
			Data.AddTopicConfigurations(Element);
		for (auto& Element : QueueConfigurations)
			Data.AddQueueConfigurations(Element);
		for (auto& Element : LambdaFunctionConfigurations)
			Data.AddLambdaFunctionConfigurations(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FTopicConfiguration> TopicConfigurations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FQueueConfiguration> QueueConfigurations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FLambdaFunctionConfiguration> LambdaFunctionConfigurations;
};

USTRUCT(BlueprintType)
struct FMetricsAndOperator
{
	GENERATED_BODY()

public:
	FMetricsAndOperator() = default;
#if !DISABLE_S3
	FMetricsAndOperator(const Aws::S3::Model::MetricsAndOperator& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
	{
		for (auto& Element : Data.GetTags())
			Tags.Add(Element);
	}

public:
	operator Aws::S3::Model::MetricsAndOperator() const
	{
		Aws::S3::Model::MetricsAndOperator Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		for (auto& Element : Tags)
			Data.AddTags(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Tag> Tags;
};

USTRUCT(BlueprintType)
struct FMetricsFilter
{
	GENERATED_BODY()

public:
	FMetricsFilter() = default;
#if !DISABLE_S3
	FMetricsFilter(const Aws::S3::Model::MetricsFilter& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Tag(Data.GetTag())
		, And(Data.GetAnd())
	{
	}

public:
	operator Aws::S3::Model::MetricsFilter() const
	{
		Aws::S3::Model::MetricsFilter Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		Data.SetTag(Tag);
		Data.SetAnd(And);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Tag Tag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FMetricsAndOperator And;
};

USTRUCT(BlueprintType)
struct FMetricsConfiguration
{
	GENERATED_BODY()

public:
	FMetricsConfiguration() = default;
#if !DISABLE_S3
	FMetricsConfiguration(const Aws::S3::Model::MetricsConfiguration& Data)
		: Id(UTF8_TO_TCHAR(Data.GetId().c_str()))
		, Filter(Data.GetFilter())
	{
	}

public:
	operator Aws::S3::Model::MetricsConfiguration() const
	{
		Aws::S3::Model::MetricsConfiguration Data = {};
		if (Id.IsEmpty() == false)
			Data.SetId(TCHAR_TO_UTF8(*Id));
		Data.SetFilter(Filter);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FMetricsFilter Filter;
};

USTRUCT(BlueprintType)
struct FTargetGrant
{
	GENERATED_BODY()

public:
	FTargetGrant()
		: Permission(EBucketLogsPermission::NOT_SET)
	{
	}
#if !DISABLE_S3
	FTargetGrant(const Aws::S3::Model::TargetGrant& Data)
		: Grantee(Data.GetGrantee())
		, Permission(static_cast<EBucketLogsPermission>(Data.GetPermission()))
	{
	}

public:
	operator Aws::S3::Model::TargetGrant() const
	{
		Aws::S3::Model::TargetGrant Data = {};
		Data.SetGrantee(Grantee);
		if (Permission != EBucketLogsPermission::NOT_SET)
			Data.SetPermission(static_cast<Aws::S3::Model::BucketLogsPermission>(Permission));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FGrantee Grantee;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketLogsPermission Permission;
};

USTRUCT(BlueprintType)
struct FLoggingEnabled
{
	GENERATED_BODY()

public:
	FLoggingEnabled() = default;
#if !DISABLE_S3
	FLoggingEnabled(const Aws::S3::Model::LoggingEnabled& Data)
		: TargetBucket(UTF8_TO_TCHAR(Data.GetTargetBucket().c_str()))
		, TargetPrefix(UTF8_TO_TCHAR(Data.GetTargetPrefix().c_str()))
	{
		for (auto& Element : Data.GetTargetGrants())
			TargetGrants.Add(Element);
	}

public:
	operator Aws::S3::Model::LoggingEnabled() const
	{
		Aws::S3::Model::LoggingEnabled Data = {};
		if (TargetBucket.IsEmpty() == false)
			Data.SetTargetBucket(TCHAR_TO_UTF8(*TargetBucket));
		for (auto& Element : TargetGrants)
			Data.AddTargetGrants(Element);
		if (TargetPrefix.IsEmpty() == false)
			Data.SetTargetPrefix(TCHAR_TO_UTF8(*TargetPrefix));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString TargetBucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FTargetGrant> TargetGrants;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString TargetPrefix;
};

USTRUCT(BlueprintType)
struct FBucketLoggingStatus
{
	GENERATED_BODY()

public:
	FBucketLoggingStatus() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::BucketLoggingStatus() const
	{
		Aws::S3::Model::BucketLoggingStatus Data = {};
		Data.SetLoggingEnabled(LoggingEnabled);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FLoggingEnabled LoggingEnabled;
};

USTRUCT(BlueprintType)
struct FLifecycleExpiration
{
	GENERATED_BODY()

public:
	FLifecycleExpiration()
		: Date(0)
		, Days(0)
		, bExpiredObjectDeleteMarker(false)
	{
	}
#if !DISABLE_S3
	FLifecycleExpiration(const Aws::S3::Model::LifecycleExpiration& Data)
		: Days(Data.GetDays())
		, bExpiredObjectDeleteMarker(Data.GetExpiredObjectDeleteMarker())
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), Date);
	}

public:
	operator Aws::S3::Model::LifecycleExpiration() const
	{
		Aws::S3::Model::LifecycleExpiration Data = {};
		if (Date > 0)
			Data.SetDate(Aws::Utils::DateTime(TCHAR_TO_UTF8(*Date.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));
		if (Days > 0)
			Data.SetDays(Days);
		Data.SetExpiredObjectDeleteMarker(bExpiredObjectDeleteMarker);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime Date;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 Days;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bExpiredObjectDeleteMarker;
};

USTRUCT(BlueprintType)
struct FLifecycleRuleAndOperator
{
	GENERATED_BODY()

public:
	FLifecycleRuleAndOperator() = default;
#if !DISABLE_S3
	FLifecycleRuleAndOperator(const Aws::S3::Model::LifecycleRuleAndOperator& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
	{
		for (auto& Element : Data.GetTags())
			Tags.Add(Element);
	}

public:
	operator Aws::S3::Model::LifecycleRuleAndOperator() const
	{
		Aws::S3::Model::LifecycleRuleAndOperator Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		for (auto& Element : Tags)
			Data.AddTags(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Tag> Tags;
};

USTRUCT(BlueprintType)
struct FLifecycleRuleFilter
{
	GENERATED_BODY()

public:
	FLifecycleRuleFilter() = default;
#if !DISABLE_S3
	FLifecycleRuleFilter(const Aws::S3::Model::LifecycleRuleFilter& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Tag(Data.GetTag())
		, And(Data.GetAnd())
	{
	}

public:
	operator Aws::S3::Model::LifecycleRuleFilter() const
	{
		Aws::S3::Model::LifecycleRuleFilter Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		Data.SetTag(Tag);
		Data.SetAnd(And);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Tag Tag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FLifecycleRuleAndOperator And;
};

USTRUCT(BlueprintType)
struct FTransition
{
	GENERATED_BODY()

public:
	FTransition()
		: Date(0)
		, Days(0)
		, StorageClass(ETransitionStorageClass::NOT_SET)
	{
	}
#if !DISABLE_S3
	FTransition(const Aws::S3::Model::Transition& Data)
		: Days(Data.GetDays())
		, StorageClass(static_cast<ETransitionStorageClass>(Data.GetStorageClass()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), Date);
	}

public:
	operator Aws::S3::Model::Transition() const
	{
		Aws::S3::Model::Transition Data = {};
		if (Date > 0)
			Data.SetDate(Aws::Utils::DateTime(TCHAR_TO_UTF8(*Date.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));
		if (Days > 0)
			Data.SetDays(Days);
		if (StorageClass != ETransitionStorageClass::NOT_SET)
			Data.SetStorageClass(static_cast<Aws::S3::Model::TransitionStorageClass>(StorageClass));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime Date;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 Days;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ETransitionStorageClass StorageClass;
};

USTRUCT(BlueprintType)
struct FNoncurrentVersionTransition
{
	GENERATED_BODY()

public:
	FNoncurrentVersionTransition()
		: NoncurrentDays(0)
		, StorageClass(ETransitionStorageClass::NOT_SET)
	{
	}
#if !DISABLE_S3
	FNoncurrentVersionTransition(const Aws::S3::Model::NoncurrentVersionTransition& Data)
		: NoncurrentDays(Data.GetNoncurrentDays())
		, StorageClass(static_cast<ETransitionStorageClass>(Data.GetStorageClass()))
	{
	}

public:
	operator Aws::S3::Model::NoncurrentVersionTransition() const
	{
		Aws::S3::Model::NoncurrentVersionTransition Data = {};
		if (NoncurrentDays > 0)
			Data.SetNoncurrentDays(NoncurrentDays);
		if (StorageClass != ETransitionStorageClass::NOT_SET)
			Data.SetStorageClass(static_cast<Aws::S3::Model::TransitionStorageClass>(StorageClass));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 NoncurrentDays;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ETransitionStorageClass StorageClass;
};

USTRUCT(BlueprintType)
struct FNoncurrentVersionExpiration
{
	GENERATED_BODY()

public:
	FNoncurrentVersionExpiration()
		: NonCurrentDays(0)
	{
	}
#if !DISABLE_S3
	FNoncurrentVersionExpiration(const Aws::S3::Model::NoncurrentVersionExpiration& Data)
		: NonCurrentDays(Data.GetNoncurrentDays())
	{
	}

public:
	operator Aws::S3::Model::NoncurrentVersionExpiration() const
	{
		Aws::S3::Model::NoncurrentVersionExpiration Data = {};
		if (NonCurrentDays > 0)
			Data.SetNoncurrentDays(NonCurrentDays);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 NonCurrentDays;
};

USTRUCT(BlueprintType)
struct FAbortIncompleteMultipartUpload
{
	GENERATED_BODY()

public:
	FAbortIncompleteMultipartUpload()
		: DaysAfterInitiation(0)
	{
	}
#if !DISABLE_S3
	FAbortIncompleteMultipartUpload(const Aws::S3::Model::AbortIncompleteMultipartUpload& Data)
		: DaysAfterInitiation(Data.GetDaysAfterInitiation())
	{
	}

public:
	operator Aws::S3::Model::AbortIncompleteMultipartUpload() const
	{
		Aws::S3::Model::AbortIncompleteMultipartUpload Data = {};
		if (DaysAfterInitiation)
			Data.SetDaysAfterInitiation(DaysAfterInitiation);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 DaysAfterInitiation;
};

USTRUCT(BlueprintType)
struct FLifecycleRule
{
	GENERATED_BODY()

public:
	FLifecycleRule()
		: Status(EExpirationStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FLifecycleRule(const Aws::S3::Model::LifecycleRule& Data)
		: Expiration(Data.GetExpiration())
		, ID(UTF8_TO_TCHAR(Data.GetID().c_str()))
		, Filter(Data.GetFilter())
		, Status(static_cast<EExpirationStatus>(Data.GetStatus()))
		, NoncurrentVersionExpiration(Data.GetNoncurrentVersionExpiration())
		, AbortIncompleteMultipartUpload(Data.GetAbortIncompleteMultipartUpload())
	{
		for (auto& Element : Data.GetTransitions())
			Transitions.Add(Element);

		for (auto& Element : Data.GetNoncurrentVersionTransitions())
			NoncurrentVersionTransitions.Add(Element);
	}

public:
	operator Aws::S3::Model::LifecycleRule() const
	{
		Aws::S3::Model::LifecycleRule Data = {};
		Data.SetExpiration(Expiration);
		if (ID.IsEmpty() == false)
			Data.SetID(TCHAR_TO_UTF8(*ID));
		Data.SetFilter(Filter);
		for (auto& Element : Transitions)
			Data.AddTransitions(Element);
		for (auto& Element : NoncurrentVersionTransitions)
			Data.AddNoncurrentVersionTransitions(Element);
		Data.SetNoncurrentVersionExpiration(NoncurrentVersionExpiration);
		Data.SetAbortIncompleteMultipartUpload(AbortIncompleteMultipartUpload);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FLifecycleExpiration Expiration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FLifecycleRuleFilter Filter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EExpirationStatus Status;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FTransition> Transitions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FNoncurrentVersionTransition> NoncurrentVersionTransitions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FNoncurrentVersionExpiration NoncurrentVersionExpiration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAbortIncompleteMultipartUpload AbortIncompleteMultipartUpload;
};

USTRUCT(BlueprintType)
struct FBucketLifecycleConfiguration
{
	GENERATED_BODY()

public:
	FBucketLifecycleConfiguration() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::BucketLifecycleConfiguration() const
	{
		Aws::S3::Model::BucketLifecycleConfiguration Data = {};
		for (auto& Element : Rules)
			Data.AddRules(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FLifecycleRule> Rules;
};

USTRUCT(BlueprintType)
struct FInventoryFilter
{
	GENERATED_BODY()

public:
	FInventoryFilter() = default;
#if !DISABLE_S3
	FInventoryFilter(const Aws::S3::Model::InventoryFilter& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
	{
	}

public:
	operator Aws::S3::Model::InventoryFilter() const
	{
		Aws::S3::Model::InventoryFilter Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
};

USTRUCT(BlueprintType)
struct FSSEKMS
{
	GENERATED_BODY()

public:
	FSSEKMS() = default;
#if !DISABLE_S3
	FSSEKMS(const Aws::S3::Model::SSEKMS& Data)
		: KeyId(UTF8_TO_TCHAR(Data.GetKeyId().c_str()))
	{
	}

public:
	operator Aws::S3::Model::SSEKMS() const
	{
		Aws::S3::Model::SSEKMS Data = {};
		if (KeyId.IsEmpty() == false)
			Data.SetKeyId(TCHAR_TO_UTF8(*KeyId));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KeyId;
};

USTRUCT(BlueprintType)
struct FInventoryEncryption
{
	GENERATED_BODY()

public:
	FInventoryEncryption() = default;
#if !DISABLE_S3
	FInventoryEncryption(const Aws::S3::Model::InventoryEncryption& Data)
		: m_SSES3(Data.GetSSES3())
		, SSEKMS(Data.GetSSEKMS())
	{
	}

public:
	operator Aws::S3::Model::InventoryEncryption() const
	{
		Aws::S3::Model::InventoryEncryption Data = {};
		Data.SetSSEKMS(SSEKMS);
		Data.SetSSES3(m_SSES3);
		return Data;
	}
#endif

public:
#if !DISABLE_S3
	Aws::S3::Model::SSES3 m_SSES3;
#endif

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FSSEKMS SSEKMS;
};

USTRUCT(BlueprintType)
struct FInventoryS3BucketDestination
{
	GENERATED_BODY()

public:
	FInventoryS3BucketDestination()
		: Format(EInventoryFormat::NOT_SET)
	{
	}
#if !DISABLE_S3
	FInventoryS3BucketDestination(const Aws::S3::Model::InventoryS3BucketDestination& Data)
		: AccountId(UTF8_TO_TCHAR(Data.GetAccountId().c_str()))
		, Bucket(UTF8_TO_TCHAR(Data.GetBucket().c_str()))
		, Format(static_cast<EInventoryFormat>(Data.GetFormat()))
		, Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Encryption(Data.GetEncryption())
	{
	}

public:
	operator Aws::S3::Model::InventoryS3BucketDestination() const
	{
		Aws::S3::Model::InventoryS3BucketDestination Data = {};
		if (AccountId.IsEmpty() == false)
			Data.SetAccountId(TCHAR_TO_UTF8(*AccountId));
		if (Bucket.IsEmpty() == false)
			Data.SetBucket(TCHAR_TO_UTF8(*Bucket));
		if (Format != EInventoryFormat::NOT_SET)
			Data.SetFormat(static_cast<Aws::S3::Model::InventoryFormat>(Format));
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		Data.SetEncryption(Encryption);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString AccountId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EInventoryFormat Format;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInventoryEncryption Encryption;
};

USTRUCT(BlueprintType)
struct FInventoryDestination
{
	GENERATED_BODY()

public:
	FInventoryDestination() = default;
#if !DISABLE_S3
	FInventoryDestination(const Aws::S3::Model::InventoryDestination& Data)
		: S3BucketDestination(Data.GetS3BucketDestination())
	{
	}

public:
	operator Aws::S3::Model::InventoryDestination() const
	{
		Aws::S3::Model::InventoryDestination Data = {};
		Data.SetS3BucketDestination(S3BucketDestination);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInventoryS3BucketDestination S3BucketDestination;
};

USTRUCT(BlueprintType)
struct FInventorySchedule
{
	GENERATED_BODY()

public:
	FInventorySchedule()
		: Frequency(EInventoryFrequency::NOT_SET)
	{
	}
#if !DISABLE_S3
	FInventorySchedule(const Aws::S3::Model::InventorySchedule& Data)
		: Frequency(static_cast<EInventoryFrequency>(Data.GetFrequency()))
	{
	}

public:
	operator Aws::S3::Model::InventorySchedule() const
	{
		Aws::S3::Model::InventorySchedule Data = {};
		if (Frequency != EInventoryFrequency::NOT_SET)
			Data.SetFrequency(static_cast<Aws::S3::Model::InventoryFrequency>(Frequency));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EInventoryFrequency Frequency;
};

USTRUCT(BlueprintType)
struct FInventoryConfiguration
{
	GENERATED_BODY()

public:
	FInventoryConfiguration()
		: bIsEnabled(false)
		, IncludedObjectVersions(EInventoryIncludedObjectVersions::NOT_SET)
	{
	}
#if !DISABLE_S3
	FInventoryConfiguration(const Aws::S3::Model::InventoryConfiguration& Data)
		: Destination(Data.GetDestination())
		, bIsEnabled(Data.GetIsEnabled())
		, Filter(Data.GetFilter())
		, Id(UTF8_TO_TCHAR(Data.GetId().c_str()))
		, IncludedObjectVersions(static_cast<EInventoryIncludedObjectVersions>(Data.GetIncludedObjectVersions()))
		, Schedule(Data.GetSchedule())
	{
		for (auto& Element : Data.GetOptionalFields())
			OptionalFields.Add(static_cast<EInventoryOptionalField>(Element));
	}

public:
	operator Aws::S3::Model::InventoryConfiguration() const
	{
		Aws::S3::Model::InventoryConfiguration Data = {};
		Data.SetDestination(Destination);
		Data.SetIsEnabled(bIsEnabled);
		Data.SetFilter(Filter);
		if (Id.IsEmpty() == false)
			Data.SetId(TCHAR_TO_UTF8(*Id));
		if (IncludedObjectVersions != EInventoryIncludedObjectVersions::NOT_SET)
			Data.SetIncludedObjectVersions(static_cast<Aws::S3::Model::InventoryIncludedObjectVersions>(IncludedObjectVersions));
		for (auto& Element : OptionalFields)
			Data.AddOptionalFields(static_cast<Aws::S3::Model::InventoryOptionalField>(Element));
		Data.SetSchedule(Schedule);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInventoryDestination Destination;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsEnabled;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInventoryFilter Filter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EInventoryIncludedObjectVersions IncludedObjectVersions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<EInventoryOptionalField> OptionalFields;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInventorySchedule Schedule;
};

USTRUCT(BlueprintType)
struct FServerSideEncryptionByDefault
{
	GENERATED_BODY()

public:
	FServerSideEncryptionByDefault()
		: SSEAlgorithm(EServerSideEncryption::NOT_SET)
	{
	}
#if !DISABLE_S3
	FServerSideEncryptionByDefault(const Aws::S3::Model::ServerSideEncryptionByDefault& Data)
		: SSEAlgorithm(static_cast<EServerSideEncryption>(Data.GetSSEAlgorithm()))
		, KMSMasterKeyID(UTF8_TO_TCHAR(Data.GetKMSMasterKeyID().c_str()))
	{
	}

public:
	operator Aws::S3::Model::ServerSideEncryptionByDefault() const
	{
		Aws::S3::Model::ServerSideEncryptionByDefault Data = {};
		if (SSEAlgorithm != EServerSideEncryption::NOT_SET)
			Data.SetSSEAlgorithm(static_cast<Aws::S3::Model::ServerSideEncryption>(SSEAlgorithm));
		if (KMSMasterKeyID.IsEmpty() == false)
			Data.SetKMSMasterKeyID(TCHAR_TO_UTF8(*KMSMasterKeyID));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption SSEAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KMSMasterKeyID;
};

USTRUCT(BlueprintType)
struct FServerSideEncryptionRule
{
	GENERATED_BODY()

public:
	FServerSideEncryptionRule() = default;
#if !DISABLE_S3
	FServerSideEncryptionRule(const Aws::S3::Model::ServerSideEncryptionRule& Data)
		: ApplyServerSideEncryptionByDefault(Data.GetApplyServerSideEncryptionByDefault())
	{
	}

public:
	operator Aws::S3::Model::ServerSideEncryptionRule() const
	{
		Aws::S3::Model::ServerSideEncryptionRule Data = {};
		Data.SetApplyServerSideEncryptionByDefault(ApplyServerSideEncryptionByDefault);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FServerSideEncryptionByDefault ApplyServerSideEncryptionByDefault;
};

USTRUCT(BlueprintType)
struct FServerSideEncryptionConfiguration
{
	GENERATED_BODY()

public:
	FServerSideEncryptionConfiguration() = default;
#if !DISABLE_S3
	FServerSideEncryptionConfiguration(const Aws::S3::Model::ServerSideEncryptionConfiguration& Data)
	{
		for (auto& Element : Data.GetRules())
			Rules.Add(Element);
	}

public:
	operator Aws::S3::Model::ServerSideEncryptionConfiguration() const
	{
		Aws::S3::Model::ServerSideEncryptionConfiguration Data = {};
		for (auto& Element : Rules)
			Data.AddRules(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FServerSideEncryptionRule> Rules;
};

USTRUCT(BlueprintType)
struct FCORSRule
{
	GENERATED_BODY()

public:
	FCORSRule()
		: MaxAgeSeconds(0)
	{
	}
#if !DISABLE_S3
	FCORSRule(const Aws::S3::Model::CORSRule& Data)
		: MaxAgeSeconds(Data.GetMaxAgeSeconds())
	{
		for (auto& Element : Data.GetAllowedHeaders())
			AllowedHeaders.Add(UTF8_TO_TCHAR(Element.c_str()));

		for (auto& Element : Data.GetAllowedMethods())
			AllowedMethods.Add(UTF8_TO_TCHAR(Element.c_str()));

		for (auto& Element : Data.GetAllowedOrigins())
			AllowedOrigins.Add(UTF8_TO_TCHAR(Element.c_str()));

		for (auto& Element : Data.GetExposeHeaders())
			ExposeHeaders.Add(UTF8_TO_TCHAR(Element.c_str()));
	}

public:
	operator Aws::S3::Model::CORSRule() const
	{
		Aws::S3::Model::CORSRule Data = {};
		for (auto& Element : AllowedHeaders)
			Data.AddAllowedHeaders(TCHAR_TO_UTF8(*Element));
		for (auto& Element : AllowedMethods)
			Data.AddAllowedMethods(TCHAR_TO_UTF8(*Element));
		for (auto& Element : AllowedOrigins)
			Data.AddAllowedOrigins(TCHAR_TO_UTF8(*Element));
		for (auto& Element : ExposeHeaders)
			Data.AddExposeHeaders(TCHAR_TO_UTF8(*Element));
		if (MaxAgeSeconds > 0)
			Data.SetMaxAgeSeconds(MaxAgeSeconds);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FString> AllowedHeaders;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FString> AllowedMethods;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FString> AllowedOrigins;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FString> ExposeHeaders;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxAgeSeconds;
};

USTRUCT(BlueprintType)
struct FCORSConfiguration
{
	GENERATED_BODY()

public:
	FCORSConfiguration() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::CORSConfiguration() const
	{
		Aws::S3::Model::CORSConfiguration Data = {};
		for (auto& Element : CORSRules)
			Data.AddCORSRules(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FCORSRule> CORSRules;
};

USTRUCT(BlueprintType)
struct FAnalyticsS3BucketDestination
{
	GENERATED_BODY()

public:
	FAnalyticsS3BucketDestination()
		: Format(EAnalyticsS3ExportFileFormat::NOT_SET)
	{
	}
#if !DISABLE_S3
	FAnalyticsS3BucketDestination(const Aws::S3::Model::AnalyticsS3BucketDestination& Data)
		: Format(static_cast<EAnalyticsS3ExportFileFormat>(Data.GetFormat()))
		, BucketAccountId(UTF8_TO_TCHAR(Data.GetBucketAccountId().c_str()))
		, Bucket(UTF8_TO_TCHAR(Data.GetBucket().c_str()))
		, Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
	{
	}

public:
	operator Aws::S3::Model::AnalyticsS3BucketDestination() const
	{
		Aws::S3::Model::AnalyticsS3BucketDestination Data = {};
		if (Format != EAnalyticsS3ExportFileFormat::NOT_SET)
			Data.SetFormat(static_cast<Aws::S3::Model::AnalyticsS3ExportFileFormat>(Format));
		if (BucketAccountId.IsEmpty() == false)
			Data.SetBucketAccountId(TCHAR_TO_UTF8(*BucketAccountId));
		if (Bucket.IsEmpty() == false)
			Data.SetBucket(TCHAR_TO_UTF8(*Bucket));
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EAnalyticsS3ExportFileFormat Format;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString BucketAccountId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
};

USTRUCT(BlueprintType)
struct FAnalyticsExportDestination
{
	GENERATED_BODY()

public:
	FAnalyticsExportDestination() = default;
#if !DISABLE_S3
	FAnalyticsExportDestination(const Aws::S3::Model::AnalyticsExportDestination& Data)
		: S3BucketDestination(Data.GetS3BucketDestination())
	{
	}

public:
	operator Aws::S3::Model::AnalyticsExportDestination() const
	{
		Aws::S3::Model::AnalyticsExportDestination Data = {};
		Data.SetS3BucketDestination(S3BucketDestination);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAnalyticsS3BucketDestination S3BucketDestination;
};

USTRUCT(BlueprintType)
struct FStorageClassAnalysisDataExport
{
	GENERATED_BODY()

public:
	FStorageClassAnalysisDataExport()
		: OutputSchemaVersion(EStorageClassAnalysisSchemaVersion::NOT_SET)
	{
	}
#if !DISABLE_S3
	FStorageClassAnalysisDataExport(const Aws::S3::Model::StorageClassAnalysisDataExport& Data)
		: OutputSchemaVersion(static_cast<EStorageClassAnalysisSchemaVersion>(Data.GetOutputSchemaVersion()))
		, Destination(Data.GetDestination())
	{
	}

public:
	operator Aws::S3::Model::StorageClassAnalysisDataExport() const
	{
		Aws::S3::Model::StorageClassAnalysisDataExport Data = {};
		if (OutputSchemaVersion != EStorageClassAnalysisSchemaVersion::NOT_SET)
			Data.SetOutputSchemaVersion(static_cast<Aws::S3::Model::StorageClassAnalysisSchemaVersion>(OutputSchemaVersion));
		Data.SetDestination(Destination);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClassAnalysisSchemaVersion OutputSchemaVersion;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAnalyticsExportDestination Destination;
};

USTRUCT(BlueprintType)
struct FStorageClassAnalysis
{
	GENERATED_BODY()

public:
	FStorageClassAnalysis() = default;
#if !DISABLE_S3
	FStorageClassAnalysis(const Aws::S3::Model::StorageClassAnalysis& Data)
		: DataExport(Data.GetDataExport())
	{
	}

public:
	operator Aws::S3::Model::StorageClassAnalysis() const
	{
		Aws::S3::Model::StorageClassAnalysis Data = {};
		Data.SetDataExport(DataExport);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FStorageClassAnalysisDataExport DataExport;
};

USTRUCT(BlueprintType)
struct FAnalyticsAndOperator
{
	GENERATED_BODY()

public:
	FAnalyticsAndOperator() = default;
#if !DISABLE_S3
	FAnalyticsAndOperator(const Aws::S3::Model::AnalyticsAndOperator& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
	{
		for (auto& Element : Data.GetTags())
			Tags.Add(Element);
	}

public:
	operator Aws::S3::Model::AnalyticsAndOperator() const
	{
		Aws::S3::Model::AnalyticsAndOperator Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		for (auto& Element : Tags)
			Data.AddTags(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Tag> Tags;
};

USTRUCT(BlueprintType)
struct FAnalyticsFilter
{
	GENERATED_BODY()

public:
	FAnalyticsFilter() = default;
#if !DISABLE_S3
	FAnalyticsFilter(const Aws::S3::Model::AnalyticsFilter& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Tag(Data.GetTag())
		, And(Data.GetAnd())
	{
	}

public:
	operator Aws::S3::Model::AnalyticsFilter() const
	{
		Aws::S3::Model::AnalyticsFilter Data = {};
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		Data.SetTag(Tag);
		Data.SetAnd(And);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Tag Tag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAnalyticsAndOperator And;
};

USTRUCT(BlueprintType)
struct FAnalyticsConfiguration
{
	GENERATED_BODY()

public:
	FAnalyticsConfiguration() = default;
#if !DISABLE_S3
	FAnalyticsConfiguration(const Aws::S3::Model::AnalyticsConfiguration& Data)
		: Id(UTF8_TO_TCHAR(Data.GetId().c_str()))
		, Filter(Data.GetFilter())
		, StorageClassAnalysis(Data.GetStorageClassAnalysis())
	{
	}

public:
	operator Aws::S3::Model::AnalyticsConfiguration() const
	{
		Aws::S3::Model::AnalyticsConfiguration Data = {};
		if (Id.IsEmpty() == false)
			Data.SetId(TCHAR_TO_UTF8(*Id));
		Data.SetFilter(Filter);
		Data.SetStorageClassAnalysis(StorageClassAnalysis);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAnalyticsFilter Filter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FStorageClassAnalysis StorageClassAnalysis;
};

USTRUCT(BlueprintType)
struct FOwner
{
	GENERATED_BODY()

public:
	FOwner() = default;
#if !DISABLE_S3
	FOwner(const Aws::S3::Model::Owner& Data)
		: DisplayName(UTF8_TO_TCHAR(Data.GetDisplayName().c_str()))
		, ID(UTF8_TO_TCHAR(Data.GetID().c_str()))
	{
	}

public:
	operator Aws::S3::Model::Owner() const
	{
		Aws::S3::Model::Owner Data = {};
		if (DisplayName.IsEmpty() == false)
			Data.SetDisplayName(TCHAR_TO_UTF8(*DisplayName));
		if (ID.IsEmpty() == false)
			Data.SetID(TCHAR_TO_UTF8(*ID));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString DisplayName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ID;
};

USTRUCT(BlueprintType)
struct FGrant
{
	GENERATED_BODY()

public:
	FGrant()
		: Permission(EPermission::NOT_SET)
	{
	}
#if !DISABLE_S3
	FGrant(const Aws::S3::Model::Grant& Data)
		: Grantee(Data.GetGrantee())
		, Permission(static_cast<EPermission>(Data.GetPermission()))
	{
	}

public:
	operator Aws::S3::Model::Grant() const
	{
		Aws::S3::Model::Grant Data = {};
		Data.SetGrantee(Grantee);
		if (Permission != EPermission::NOT_SET)
			Data.SetPermission(static_cast<Aws::S3::Model::Permission>(Permission));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FGrantee Grantee;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EPermission Permission;
};

USTRUCT(BlueprintType)
struct FAccessControlPolicy
{
	GENERATED_BODY()

public:
	FAccessControlPolicy() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::AccessControlPolicy() const
	{
		Aws::S3::Model::AccessControlPolicy Data = {};
		for (auto& Element : Grants)
			Data.AddGrants(Element);
		Data.SetOwner(Owner);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FGrant> Grants;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
};

USTRUCT(BlueprintType)
struct FAccelerateConfiguration
{
	GENERATED_BODY()

public:
	FAccelerateConfiguration()
		: Status(EBucketAccelerateStatus::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::AccelerateConfiguration() const
	{
		Aws::S3::Model::AccelerateConfiguration Data = {};
		if (Status != EBucketAccelerateStatus::NOT_SET)
			Data.SetStatus(static_cast<Aws::S3::Model::BucketAccelerateStatus>(Status));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketAccelerateStatus Status;
};

USTRUCT(BlueprintType)
struct FObjectIdentifier
{
	GENERATED_BODY()

public:
	FObjectIdentifier() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::ObjectIdentifier() const
	{
		Aws::S3::Model::ObjectIdentifier Data = {};
		if (Key.IsEmpty() == false)
			Data.SetKey(TCHAR_TO_UTF8(*Key));
		if (VersionId.IsEmpty() == false)
			Data.SetVersionId(TCHAR_TO_UTF8(*VersionId));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
};

USTRUCT(BlueprintType)
struct FS3Delete
{
	GENERATED_BODY()

public:
	FS3Delete()
		: bQuiet(false)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::Delete() const
	{
		Aws::S3::Model::Delete Data = {};
		for (auto& Element : Objects)
			Data.AddObjects(Element);
		Data.SetQuiet(bQuiet);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FObjectIdentifier> Objects;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bQuiet;
};

USTRUCT(BlueprintType)
struct FCreateBucketConfiguration
{
	GENERATED_BODY()

public:
	FCreateBucketConfiguration()
		: LocationConstraint(EBucketLocationConstraint::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::CreateBucketConfiguration() const
	{
		Aws::S3::Model::CreateBucketConfiguration Data = {};
		if (LocationConstraint != EBucketLocationConstraint::NOT_SET)
			Data.SetLocationConstraint(static_cast<Aws::S3::Model::BucketLocationConstraint>(LocationConstraint));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketLocationConstraint LocationConstraint;
};

USTRUCT(BlueprintType)
struct FCompletedPart
{
	GENERATED_BODY()

public:
	FCompletedPart()
		: PartNumber(0)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::CompletedPart() const
	{
		Aws::S3::Model::CompletedPart Data = {};
		if (ETag.IsEmpty() == false)
			Data.SetETag(TCHAR_TO_UTF8(*ETag));
		if (PartNumber > 0)
			Data.SetPartNumber(PartNumber);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumber;
};

USTRUCT(BlueprintType)
struct FCompletedMultipartUpload
{
	GENERATED_BODY()

public:
	FCompletedMultipartUpload() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::CompletedMultipartUpload() const
	{
		Aws::S3::Model::CompletedMultipartUpload Data = {};
		for (auto& Element : Parts)
			Data.AddParts(Element);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FCompletedPart> Parts;
};

USTRUCT(BlueprintType)
struct FAbortMultipartUploadRequest
{
	GENERATED_BODY()

public:
	FAbortMultipartUploadRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FCompleteMultipartUploadRequest
{
	GENERATED_BODY()

public:
	FCompleteMultipartUploadRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FCompletedMultipartUpload MultipartUpload;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FCopyObjectRequest
{
	GENERATED_BODY()

public:
	FCopyObjectRequest()
		: ACL(EObjectCannedACL::NOT_SET)
		, CopySourceIfModifiedSince(0)
		, CopySourceIfUnmodifiedSince(0)
		, Expires(0)
		, MetadataDirective(EMetadataDirective::NOT_SET)
		, TaggingDirective(ETaggingDirective::NOT_SET)
		, ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, StorageClass(EStorageClass::NOT_SET)
		, RequestPayer(ERequestPayer::NOT_SET)
		, ObjectLockMode(EObjectLockMode::NOT_SET)
		, ObjectLockRetainUntilDate(0)
		, ObjectLockLegalHoldStatus(EObjectLockLegalHoldStatus::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectCannedACL ACL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CacheControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentDisposition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentEncoding;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentLanguage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySource;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceIfMatch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime CopySourceIfModifiedSince;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceIfNoneMatch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime CopySourceIfUnmodifiedSince;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime Expires;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantFullControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantReadACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWriteACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> Metadata;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EMetadataDirective MetadataDirective;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ETaggingDirective TaggingDirective;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString WebsiteRedirectLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSEncryptionContext;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceSSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceSSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceSSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Tagging;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockMode ObjectLockMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime ObjectLockRetainUntilDate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockLegalHoldStatus ObjectLockLegalHoldStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FCreateBucketRequest
{
	GENERATED_BODY()

public:
	FCreateBucketRequest()
		: ACL(EBucketCannedACL::NOT_SET)
		, bObjectLockEnabledForBucket(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketCannedACL ACL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FCreateBucketConfiguration CreateBucketConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantFullControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantReadACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWrite;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWriteACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bObjectLockEnabledForBucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FCreateMultipartUploadRequest
{
	GENERATED_BODY()

public:
	FCreateMultipartUploadRequest()
		: ACL(EObjectCannedACL::NOT_SET)
		, Expires(0)
		, ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, StorageClass(EStorageClass::NOT_SET)
		, RequestPayer(ERequestPayer::NOT_SET)
		, ObjectLockMode(EObjectLockMode::NOT_SET)
		, ObjectLockRetainUntilDate(0)
		, ObjectLockLegalHoldStatus(EObjectLockLegalHoldStatus::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectCannedACL ACL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CacheControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentDisposition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentEncoding;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentLanguage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime Expires;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantFullControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantReadACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWriteACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> Metadata;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString WebsiteRedirectLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSEncryptionContext;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Tagging;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockMode ObjectLockMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime ObjectLockRetainUntilDate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockLegalHoldStatus ObjectLockLegalHoldStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketAnalyticsConfigurationRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketAnalyticsConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketCorsRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketCorsRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FEventBridgeConfiguration
{
	GENERATED_BODY()

public:
	FEventBridgeConfiguration() = default;
};

USTRUCT(BlueprintType)
struct FDeleteBucketEncryptionRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketEncryptionRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FObjectPart
{
	GENERATED_BODY()

public:
	FObjectPart()
		: PartNumber(0)
		, Size(0)
	{
	}

#if !DISABLE_S3
	FObjectPart(const Aws::S3::Model::ObjectPart& Data)
		: PartNumber(Data.GetPartNumber())
		, Size(Data.GetSize())
		, ChecksumCRC32(UTF8_TO_TCHAR(Data.GetChecksumCRC32().c_str()))
		, ChecksumCRC32C(UTF8_TO_TCHAR(Data.GetChecksumCRC32C().c_str()))
		, ChecksumSHA1(UTF8_TO_TCHAR(Data.GetChecksumSHA1().c_str()))
		, ChecksumSHA256(UTF8_TO_TCHAR(Data.GetChecksumSHA256().c_str()))
	{
	}
	
	operator Aws::S3::Model::ObjectPart() const
	{
		Aws::S3::Model::ObjectPart Data = {};
		if (PartNumber > 0)
			Data.SetPartNumber(PartNumber);
		if (Size > 0)
			Data.SetSize(Size);
		if (ChecksumCRC32.IsEmpty() == false)
			Data.SetChecksumCRC32(TCHAR_TO_UTF8(*ChecksumCRC32));
		if (ChecksumCRC32C.IsEmpty() == false)
			Data.SetChecksumCRC32C(TCHAR_TO_UTF8(*ChecksumCRC32C));
		if (ChecksumSHA1.IsEmpty() == false)
			Data.SetChecksumSHA1(TCHAR_TO_UTF8(*ChecksumSHA1));
		if (ChecksumSHA256.IsEmpty() == false)
			Data.SetChecksumSHA256(TCHAR_TO_UTF8(*ChecksumSHA256));

		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumber;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int64 Size;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ChecksumCRC32;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ChecksumCRC32C;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ChecksumSHA1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ChecksumSHA256;
};

USTRUCT(BlueprintType)
struct FGetObjectAttributesParts
{
	GENERATED_BODY()

public:
	FGetObjectAttributesParts()
		: TotalPartsCount(0)
		, PartNumberMarker(0)
		, NextPartNumberMarker(0)
		, MaxParts(0)
		, IsTruncated(false)
	{
	}

#if !DISABLE_S3
	FGetObjectAttributesParts(const Aws::S3::Model::GetObjectAttributesParts& Data)
		: TotalPartsCount(Data.GetTotalPartsCount())
		, PartNumberMarker(Data.GetPartNumberMarker())
		, NextPartNumberMarker(Data.GetNextPartNumberMarker())
		, MaxParts(Data.GetMaxParts())
		, IsTruncated(Data.GetIsTruncated())
	{
		for (auto& Element : Data.GetParts())
			Parts.Add(Element);
	}
	
	operator Aws::S3::Model::GetObjectAttributesParts() const
	{
		Aws::S3::Model::GetObjectAttributesParts Data = {};
		if (TotalPartsCount > 0)
			Data.SetTotalPartsCount(TotalPartsCount);
		if (PartNumberMarker > 0)
			Data.SetPartNumberMarker(PartNumberMarker);
		if (NextPartNumberMarker > 0)
			Data.SetNextPartNumberMarker(NextPartNumberMarker);
		if (MaxParts > 0)
			Data.SetMaxParts(MaxParts);
		Data.SetIsTruncated(IsTruncated);

		for (auto& Element : Parts)
			Data.AddParts(Element);
		
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 TotalPartsCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumberMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 NextPartNumberMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxParts;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool IsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FObjectPart> Parts;
};

USTRUCT(BlueprintType)
struct FGetObjectAttributesRequest
{
	GENERATED_BODY()

public:
	FGetObjectAttributesRequest()
		: MaxParts(0)
		, PartNumberMarker(0)
		, RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxParts;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumberMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ExpectedBucketOwner;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<EObjectAttributes> ObjectAttributes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketInventoryConfigurationRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketInventoryConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketLifecycleRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketLifecycleRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketMetricsConfigurationRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketMetricsConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketPolicyRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketPolicyRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketReplicationRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketReplicationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketTaggingRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketTaggingRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteBucketWebsiteRequest
{
	GENERATED_BODY()

public:
	FDeleteBucketWebsiteRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteObjectRequest
{
	GENERATED_BODY()

public:
	FDeleteObjectRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
		, bBypassGovernanceRetention(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString MFA;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bBypassGovernanceRetention;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteObjectTaggingRequest
{
	GENERATED_BODY()

public:
	FDeleteObjectTaggingRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeleteObjectsRequest
{
	GENERATED_BODY()

public:
	FDeleteObjectsRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
		, bBypassGovernanceRetention(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Delete Delete;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString MFA;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bBypassGovernanceRetention;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FDeletePublicAccessBlockRequest
{
	GENERATED_BODY()

public:
	FDeletePublicAccessBlockRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketAccelerateConfigurationRequest
{
	GENERATED_BODY()

public:
	FGetBucketAccelerateConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketAclRequest
{
	GENERATED_BODY()

public:
	FGetBucketAclRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketAnalyticsConfigurationRequest
{
	GENERATED_BODY()

public:
	FGetBucketAnalyticsConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketCorsRequest
{
	GENERATED_BODY()

public:
	FGetBucketCorsRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketEncryptionRequest
{
	GENERATED_BODY()

public:
	FGetBucketEncryptionRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketInventoryConfigurationRequest
{
	GENERATED_BODY()

public:
	FGetBucketInventoryConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketLifecycleConfigurationRequest
{
	GENERATED_BODY()

public:
	FGetBucketLifecycleConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketLocationRequest
{
	GENERATED_BODY()

public:
	FGetBucketLocationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketLoggingRequest
{
	GENERATED_BODY()

public:
	FGetBucketLoggingRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketMetricsConfigurationRequest
{
	GENERATED_BODY()

public:
	FGetBucketMetricsConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketNotificationConfigurationRequest
{
	GENERATED_BODY()

public:
	FGetBucketNotificationConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketPolicyRequest
{
	GENERATED_BODY()

public:
	FGetBucketPolicyRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketPolicyStatusRequest
{
	GENERATED_BODY()

public:
	FGetBucketPolicyStatusRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketReplicationRequest
{
	GENERATED_BODY()

public:
	FGetBucketReplicationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketRequestPaymentRequest
{
	GENERATED_BODY()

public:
	FGetBucketRequestPaymentRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketTaggingRequest
{
	GENERATED_BODY()

public:
	FGetBucketTaggingRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketVersioningRequest
{
	GENERATED_BODY()

public:
	FGetBucketVersioningRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetBucketWebsiteRequest
{
	GENERATED_BODY()

public:
	FGetBucketWebsiteRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetObjectRequest
{
	GENERATED_BODY()

public:
	FGetObjectRequest()
		: IfModifiedSince(0)
		, IfUnmodifiedSince(0)
		, ResponseExpires(0)
		, RequestPayer(ERequestPayer::NOT_SET)
		, PartNumber(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString IfMatch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime IfModifiedSince;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString IfNoneMatch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime IfUnmodifiedSince;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Range;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ResponseCacheControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ResponseContentDisposition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ResponseContentEncoding;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ResponseContentLanguage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ResponseContentType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime ResponseExpires;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumber;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString FileSavePath;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CustomFileName;
};

USTRUCT(BlueprintType)
struct FGetObjectAclRequest
{
	GENERATED_BODY()

public:
	FGetObjectAclRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetObjectLegalHoldRequest
{
	GENERATED_BODY()

public:
	FGetObjectLegalHoldRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetObjectLockConfigurationRequest
{
	GENERATED_BODY()

public:
	FGetObjectLockConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetObjectRetentionRequest
{
	GENERATED_BODY()

public:
	FGetObjectRetentionRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetObjectTaggingRequest
{
	GENERATED_BODY()

public:
	FGetObjectTaggingRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetObjectTorrentRequest
{
	GENERATED_BODY()

public:
	FGetObjectTorrentRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	/* Full path to where we save the torrent file */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString File;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FGetPublicAccessBlockRequest
{
	GENERATED_BODY()

public:
	FGetPublicAccessBlockRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FHeadBucketRequest
{
	GENERATED_BODY()

public:
	FHeadBucketRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FHeadObjectRequest
{
	GENERATED_BODY()

public:
	FHeadObjectRequest()
		: IfModifiedSince(0)
		, IfUnmodifiedSince(0)
		, RequestPayer(ERequestPayer::NOT_SET)
		, PartNumber(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString IfMatch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime IfModifiedSince;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString IfNoneMatch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime IfUnmodifiedSince;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Range;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumber;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FListBucketAnalyticsConfigurationsRequest
{
	GENERATED_BODY()

public:
	FListBucketAnalyticsConfigurationsRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FListBucketInventoryConfigurationsRequest
{
	GENERATED_BODY()

public:
	FListBucketInventoryConfigurationsRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FListBucketMetricsConfigurationsRequest
{
	GENERATED_BODY()

public:
	FListBucketMetricsConfigurationsRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FListMultipartUploadsRequest
{
	GENERATED_BODY()

public:
	FListMultipartUploadsRequest()
		: EncodingType(EEncodingType::NOT_SET)
		, MaxUploads(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Delimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EEncodingType EncodingType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KeyMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxUploads;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadIdMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FListObjectVersionsRequest
{
	GENERATED_BODY()

public:
	FListObjectVersionsRequest()
		: EncodingType(EEncodingType::NOT_SET)
		, MaxKeys(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Delimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EEncodingType EncodingType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KeyMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxKeys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionIdMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FListObjectsRequest
{
	GENERATED_BODY()

public:
	FListObjectsRequest()
		: EncodingType(EEncodingType::NOT_SET)
		, MaxKeys(0)
		, RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Delimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EEncodingType EncodingType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Marker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxKeys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FListObjectsV2Request
{
	GENERATED_BODY()

public:
	FListObjectsV2Request()
		: EncodingType(EEncodingType::NOT_SET)
		, MaxKeys(0)
		, bFetchOwner(0)
		, RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Delimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EEncodingType EncodingType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxKeys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bFetchOwner;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString StartAfter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FListPartsRequest
{
	GENERATED_BODY()

public:
	FListPartsRequest()
		: MaxParts(0)
		, PartNumberMarker(0)
		, RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxParts;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumberMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketAccelerateConfigurationRequest
{
	GENERATED_BODY()

public:
	FPutBucketAccelerateConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAccelerateConfiguration AccelerateConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketAclRequest
{
	GENERATED_BODY()

public:
	FPutBucketAclRequest()
		: ACL(EBucketCannedACL::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketCannedACL ACL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAccessControlPolicy AccessControlPolicy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantFullControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantReadACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWrite;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWriteACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketAnalyticsConfigurationRequest
{
	GENERATED_BODY()

public:
	FPutBucketAnalyticsConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAnalyticsConfiguration AnalyticsConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketCorsRequest
{
	GENERATED_BODY()

public:
	FPutBucketCorsRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FCORSConfiguration CORSConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketEncryptionRequest
{
	GENERATED_BODY()

public:
	FPutBucketEncryptionRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FServerSideEncryptionConfiguration ServerSideEncryptionConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketInventoryConfigurationRequest
{
	GENERATED_BODY()

public:
	FPutBucketInventoryConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInventoryConfiguration InventoryConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketLifecycleConfigurationRequest
{
	GENERATED_BODY()

public:
	FPutBucketLifecycleConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FBucketLifecycleConfiguration LifecycleConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketLoggingRequest
{
	GENERATED_BODY()

public:
	FPutBucketLoggingRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FBucketLoggingStatus BucketLoggingStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketMetricsConfigurationRequest
{
	GENERATED_BODY()

public:
	FPutBucketMetricsConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FMetricsConfiguration MetricsConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketNotificationConfigurationRequest
{
	GENERATED_BODY()

public:
	FPutBucketNotificationConfigurationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FNotificationConfiguration NotificationConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketPolicyRequest
{
	GENERATED_BODY()

public:
	FPutBucketPolicyRequest()
		: bConfirmRemoveSelfBucketAccess(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bConfirmRemoveSelfBucketAccess;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketReplicationRequest
{
	GENERATED_BODY()

public:
	FPutBucketReplicationRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FReplicationConfiguration ReplicationConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Token;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketRequestPaymentRequest
{
	GENERATED_BODY()

public:
	FPutBucketRequestPaymentRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FRequestPaymentConfiguration RequestPaymentConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketTaggingRequest
{
	GENERATED_BODY()

public:
	FPutBucketTaggingRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Tagging Tagging;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketVersioningRequest
{
	GENERATED_BODY()

public:
	FPutBucketVersioningRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString MFA;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FVersioningConfiguration VersioningConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutBucketWebsiteRequest
{
	GENERATED_BODY()

public:
	FPutBucketWebsiteRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FWebsiteConfiguration WebsiteConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutObjectRequest
{
	GENERATED_BODY()

public:
	FPutObjectRequest()
		: ACL(EObjectCannedACL::NOT_SET)
		, Expires(0)
		, ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, StorageClass(EStorageClass::NOT_SET)
		, RequestPayer(ERequestPayer::NOT_SET)
		, ObjectLockMode(EObjectLockMode::NOT_SET)
		, ObjectLockRetainUntilDate(0)
		, ObjectLockLegalHoldStatus(EObjectLockLegalHoldStatus::NOT_SET)
	{
	}

public:
	/* Full path to the file that we want to upload */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString File;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectCannedACL ACL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CacheControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentDisposition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentEncoding;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentLanguage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime Expires;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantFullControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantReadACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWriteACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> Metadata;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString WebsiteRedirectLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSEncryptionContext;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Tagging;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockMode ObjectLockMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime ObjectLockRetainUntilDate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockLegalHoldStatus ObjectLockLegalHoldStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutObjectAclRequest
{
	GENERATED_BODY()

public:
	FPutObjectAclRequest()
		: ACL(EObjectCannedACL::NOT_SET)
		, RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectCannedACL ACL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAccessControlPolicy AccessControlPolicy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantFullControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantRead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantReadACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWrite;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString GrantWriteACP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutObjectLegalHoldRequest
{
	GENERATED_BODY()

public:
	FPutObjectLegalHoldRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FObjectLockLegalHold LegalHold;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutObjectLockConfigurationRequest
{
	GENERATED_BODY()

public:
	FPutObjectLockConfigurationRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FObjectLockConfiguration ObjectLockConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Token;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutObjectRetentionRequest
{
	GENERATED_BODY()

public:
	FPutObjectRetentionRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
		, bBypassGovernanceRetention(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FObjectLockRetention Retention;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bBypassGovernanceRetention;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutObjectTaggingRequest
{
	GENERATED_BODY()

public:
	FPutObjectTaggingRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Tagging Tagging;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FPutPublicAccessBlockRequest
{
	GENERATED_BODY()

public:
	FPutPublicAccessBlockRequest() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FPublicAccessBlockConfiguration PublicAccessBlockConfiguration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FJSONInput
{
	GENERATED_BODY()

public:
	FJSONInput()
		: Type(EJSONType::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::JSONInput() const
	{
		Aws::S3::Model::JSONInput Data = {};
		if (Type != EJSONType::NOT_SET)
			Data.SetType(static_cast<Aws::S3::Model::JSONType>(Type));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EJSONType Type;
};

USTRUCT(BlueprintType)
struct FCSVInput
{
	GENERATED_BODY()

public:
	FCSVInput()
		: FileHeaderInfo(EFileHeaderInfo::NONE)
		, bAllowQuotedRecordDelimiter(false)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::CSVInput() const
	{
		Aws::S3::Model::CSVInput Data = {};
		if (FileHeaderInfo != EFileHeaderInfo::NOT_SET)
			Data.SetFileHeaderInfo(static_cast<Aws::S3::Model::FileHeaderInfo>(FileHeaderInfo));
		if (Comments.IsEmpty() == false)
			Data.SetComments(TCHAR_TO_UTF8(*Comments));
		if (QuoteEscapeCharacter.IsEmpty() == false)
			Data.SetQuoteEscapeCharacter(TCHAR_TO_UTF8(*QuoteEscapeCharacter));
		if (RecordDelimiter.IsEmpty() == false)
			Data.SetRecordDelimiter(TCHAR_TO_UTF8(*RecordDelimiter));
		if (FieldDelimiter.IsEmpty() == false)
			Data.SetFieldDelimiter(TCHAR_TO_UTF8(*FieldDelimiter));
		if (QuoteCharacter.IsEmpty() == false)
			Data.SetQuoteCharacter(TCHAR_TO_UTF8(*QuoteCharacter));
		Data.SetAllowQuotedRecordDelimiter(bAllowQuotedRecordDelimiter);

		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EFileHeaderInfo FileHeaderInfo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Comments;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString QuoteEscapeCharacter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString RecordDelimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString FieldDelimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString QuoteCharacter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bAllowQuotedRecordDelimiter;
};

USTRUCT(BlueprintType)
struct FJSONOutput
{
	GENERATED_BODY()

public:
	FJSONOutput() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::JSONOutput() const
	{
		Aws::S3::Model::JSONOutput Data = {};
		if (RecordDelimiter.IsEmpty() == false)
			Data.SetRecordDelimiter(TCHAR_TO_UTF8(*RecordDelimiter));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString RecordDelimiter;
};

USTRUCT(BlueprintType)
struct FCSVOutput
{
	GENERATED_BODY()

public:
	FCSVOutput()
		: QuoteFields(EQuoteFields::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::CSVOutput() const
	{
		Aws::S3::Model::CSVOutput Data = {};
		if (QuoteFields != EQuoteFields::NOT_SET)
			Data.SetQuoteFields(static_cast<Aws::S3::Model::QuoteFields>(QuoteFields));
		if (QuoteEscapeCharacter.IsEmpty() == false)
			Data.SetQuoteEscapeCharacter(TCHAR_TO_UTF8(*QuoteEscapeCharacter));
		if (RecordDelimiter.IsEmpty() == false)
			Data.SetRecordDelimiter(TCHAR_TO_UTF8(*RecordDelimiter));
		if (FieldDelimiter.IsEmpty() == false)
			Data.SetFieldDelimiter(TCHAR_TO_UTF8(*FieldDelimiter));
		if (QuoteCharacter.IsEmpty() == false)
			Data.SetQuoteCharacter(TCHAR_TO_UTF8(*QuoteCharacter));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EQuoteFields QuoteFields;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString QuoteEscapeCharacter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString RecordDelimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString FieldDelimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString QuoteCharacter;
};

USTRUCT(BlueprintType)
struct FOutputSerialization
{
	GENERATED_BODY()

public:
	FOutputSerialization() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::OutputSerialization() const
	{
		Aws::S3::Model::OutputSerialization Data = {};
		Data.SetCSV(CSV);
		Data.SetJSON(JSON);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FCSVOutput CSV;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FJSONOutput JSON;
};

USTRUCT(BlueprintType)
struct FInputSerialization
{
	GENERATED_BODY()

public:
	FInputSerialization()
		: CompressionType(ECompressionType::NONE)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::InputSerialization() const
	{
		Aws::S3::Model::InputSerialization Data = {};
		Data.SetCSV(CSV);
		if (CompressionType != ECompressionType::NOT_SET)
			Data.SetCompressionType(static_cast<Aws::S3::Model::CompressionType>(CompressionType));
		Data.SetJSON(JSON);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FCSVInput CSV;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ECompressionType CompressionType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FJSONInput JSON;

	//	ParquetInput m_parquet;
};

USTRUCT(BlueprintType)
struct FSelectParameters
{
	GENERATED_BODY()

public:
	FSelectParameters()
		: ExpressionType(ES3ExpressionType::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::SelectParameters() const
	{
		Aws::S3::Model::SelectParameters Data = {};
		Data.SetInputSerialization(InputSerialization);
		if (ExpressionType != ES3ExpressionType::NOT_SET)
			Data.SetExpressionType(static_cast<Aws::S3::Model::ExpressionType>(ExpressionType));
		if (Expression.IsEmpty() == false)
			Data.SetExpression(TCHAR_TO_UTF8(*Expression));
		Data.SetOutputSerialization(OutputSerialization);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInputSerialization InputSerialization;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ES3ExpressionType ExpressionType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Expression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOutputSerialization OutputSerialization;
};

USTRUCT(BlueprintType)
struct FGlacierJobParameters
{
	GENERATED_BODY()

public:
	FGlacierJobParameters()
		: Tier(ETier::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::GlacierJobParameters() const
	{
		Aws::S3::Model::GlacierJobParameters Data = {};
		if (Tier != ETier::NOT_SET)
			Data.SetTier(static_cast<Aws::S3::Model::Tier>(Tier));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ETier Tier;
};

USTRUCT(BlueprintType)
struct FMetadataEntry
{
	GENERATED_BODY()

public:
	FMetadataEntry() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::MetadataEntry() const
	{
		Aws::S3::Model::MetadataEntry Data = {};
		if (Name.IsEmpty() == false)
			Data.SetName(TCHAR_TO_UTF8(*Name));
		if (Value.IsEmpty() == false)
			Data.SetValue(TCHAR_TO_UTF8(*Value));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Value;
};

USTRUCT(BlueprintType)
struct FEncryption
{
	GENERATED_BODY()

public:
	FEncryption()
		: EncryptionType(EServerSideEncryption::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::Encryption() const
	{
		Aws::S3::Model::Encryption Data = {};
		if (EncryptionType != EServerSideEncryption::NOT_SET)
			Data.SetEncryptionType(static_cast<Aws::S3::Model::ServerSideEncryption>(EncryptionType));
		if (KMSKeyId.IsEmpty() == false)
			Data.SetKMSKeyId(TCHAR_TO_UTF8(*KMSKeyId));
		if (KMSContext.IsEmpty() == false)
			Data.SetKMSContext(TCHAR_TO_UTF8(*KMSContext));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption EncryptionType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KMSContext;
};

USTRUCT(BlueprintType)
struct FFFS3Location
{
	GENERATED_BODY()

public:
	FFFS3Location()
		: CannedACL(EObjectCannedACL::NOT_SET)
		, StorageClass(EStorageClass::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::S3Location() const
	{
		Aws::S3::Model::S3Location Data = {};
		if (BucketName.IsEmpty() == false)
			Data.SetBucketName(TCHAR_TO_UTF8(*BucketName));
		if (Prefix.IsEmpty() == false)
			Data.SetPrefix(TCHAR_TO_UTF8(*Prefix));
		Data.SetEncryption(Encryption);
		if (CannedACL != EObjectCannedACL::NOT_SET)
			Data.SetCannedACL(static_cast<Aws::S3::Model::ObjectCannedACL>(CannedACL));
		for (auto& Element : AccessControlList)
			Data.AddAccessControlList(Element);
		Data.SetTagging(Tagging);
		for (auto& Element : UserMetadata)
			Data.AddUserMetadata(Element);
		if (StorageClass != EStorageClass::NOT_SET)
			Data.SetStorageClass(static_cast<Aws::S3::Model::StorageClass>(StorageClass));
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString BucketName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FEncryption Encryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectCannedACL CannedACL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FGrant> AccessControlList;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FS3Tagging Tagging;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FMetadataEntry> UserMetadata;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
};

USTRUCT(BlueprintType)
struct FOutputLocation
{
	GENERATED_BODY()

public:
	FOutputLocation() = default;

public:
#if !DISABLE_S3
	operator Aws::S3::Model::OutputLocation() const
	{
		Aws::S3::Model::OutputLocation Data = {};
		Data.SetS3(S3);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FFFS3Location S3;
};

USTRUCT(BlueprintType)
struct FRestoreRequest
{
	GENERATED_BODY()

public:
	FRestoreRequest()
		: Days(0)
		, Type(ERestoreRequestType::NOT_SET)
		, Tier(ETier::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	operator Aws::S3::Model::RestoreRequest() const
	{
		Aws::S3::Model::RestoreRequest Data = {};
		if (Days > 0)
			Data.SetDays(Days);
		Data.SetGlacierJobParameters(GlacierJobParameters);
		if (Type != ERestoreRequestType::NOT_SET)
			Data.SetType(static_cast<Aws::S3::Model::RestoreRequestType>(Type));
		if (Tier != ETier::NOT_SET)
			Data.SetTier(static_cast<Aws::S3::Model::Tier>(Tier));
		if (Description.IsEmpty() == false)
			Data.SetDescription(TCHAR_TO_UTF8(*Description));
		Data.SetSelectParameters(SelectParameters);
		Data.SetOutputLocation(OutputLocation);
		return Data;
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 Days;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FGlacierJobParameters GlacierJobParameters;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERestoreRequestType Type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ETier Tier;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Description;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FSelectParameters SelectParameters;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOutputLocation OutputLocation;
};

USTRUCT(BlueprintType)
struct FRestoreObjectRequest
{
	GENERATED_BODY()

public:
	FRestoreObjectRequest()
		: RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FRestoreRequest RestoreRequest;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FScanRange
{
	GENERATED_BODY()

public:
	FScanRange() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Start;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString End;
};

USTRUCT(BlueprintType)
struct FRequestProgress
{
	GENERATED_BODY()

public:
	FRequestProgress()
		: bEnabled(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bEnabled;
};

USTRUCT(BlueprintType)
struct FSelectObjectContentRequest
{
	GENERATED_BODY()

public:
	FSelectObjectContentRequest()
		: ExpressionType(ES3ExpressionType::NOT_SET)
		, m_handler(nullptr)
		, m_decoder(nullptr)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Expression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ES3ExpressionType ExpressionType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FRequestProgress RequestProgress;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInputSerialization InputSerialization;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOutputSerialization OutputSerialization;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FScanRange ScanRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;

#if !DISABLE_S3
	Aws::S3::Model::SelectObjectContentHandler* m_handler;
	Aws::Utils::Event::EventStreamDecoder* m_decoder;
#else
	void* m_handler;
	void* m_decoder;
#endif
};

USTRUCT(BlueprintType)
struct FUploadPartRequest
{
	GENERATED_BODY()

public:
	FUploadPartRequest()
		: PartNumber(0)
		, RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	/* Full path to file we want to upload */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString File;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumber;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FUploadPartCopyRequest
{
	GENERATED_BODY()

public:
	FUploadPartCopyRequest()
		: CopySourceIfModifiedSince(0)
		, CopySourceIfUnmodifiedSince(0)
		, PartNumber(0)
		, RequestPayer(ERequestPayer::NOT_SET)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySource;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceIfMatch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime CopySourceIfModifiedSince;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceIfNoneMatch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime CopySourceIfUnmodifiedSince;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumber;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceSSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceSSECustomerKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceSSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestPayer RequestPayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> CustomizedAccessLogTag;
};

USTRUCT(BlueprintType)
struct FCopyPartResult
{
	GENERATED_BODY()

public:
	FCopyPartResult()
		: LastModified(0)
	{
	}

#if !DISABLE_S3
	FCopyPartResult(const Aws::S3::Model::CopyPartResult& Data)
		: ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastModified().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastModified);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime LastModified;
};

USTRUCT(BlueprintType)
struct FS3Part
{
	GENERATED_BODY()

public:
	FS3Part()
		: PartNumber(0)
		, LastModified(0)
		, Size(0)
	{
	}
#if !DISABLE_S3
	FS3Part(const Aws::S3::Model::Part& Data)
		: PartNumber(Data.GetPartNumber())
		, ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, Size(Data.GetSize())
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastModified().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastModified);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumber;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime LastModified;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int64 Size;
};


USTRUCT(BlueprintType)
struct FS3Object
{
	GENERATED_BODY()

public:
	FS3Object()
		: LastModified(0)
		, Size(0)
		, StorageClass(EObjectStorageClass::NOT_SET)
	{
	}
#if !DISABLE_S3
	FS3Object(const Aws::S3::Model::Object& Data)
		: Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, Size(Data.GetSize())
		, StorageClass(static_cast<EObjectStorageClass>(Data.GetStorageClass()))
		, Owner(Data.GetOwner())
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastModified().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastModified);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime LastModified;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int64 Size;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
};


USTRUCT(BlueprintType)
struct FDeleteMarkerEntry
{
	GENERATED_BODY()

public:
	FDeleteMarkerEntry()
		: bIsLatest(false)
		, LastModified(0)
	{
	}
#if !DISABLE_S3
	FDeleteMarkerEntry(const Aws::S3::Model::DeleteMarkerEntry& Data)
		: Owner(Data.GetOwner())
		, Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, bIsLatest(Data.GetIsLatest())
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastModified().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastModified);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsLatest;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime LastModified;
};

USTRUCT(BlueprintType)
struct FObjectVersion
{
	GENERATED_BODY()

public:
	FObjectVersion()
		: Size(0)
		, StorageClass(EObjectVersionStorageClass::NOT_SET)
		, bIsLatest(false)
		, LastModified(0)
	{
	}
#if !DISABLE_S3
	FObjectVersion(const Aws::S3::Model::ObjectVersion& Data)
		: ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, Size(Data.GetSize())
		, StorageClass(static_cast<EObjectVersionStorageClass>(Data.GetStorageClass()))
		, Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, bIsLatest(Data.GetIsLatest())
		, Owner(Data.GetOwner())
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastModified().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastModified);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int64 Size;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectVersionStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsLatest;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime LastModified;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
};

USTRUCT(BlueprintType)
struct FCommonPrefix
{
	GENERATED_BODY()

public:
	FCommonPrefix() = default;
#if !DISABLE_S3
	FCommonPrefix(const Aws::S3::Model::CommonPrefix& Data)
		: Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
};

USTRUCT(BlueprintType)
struct FInitiator
{
	GENERATED_BODY()

public:
	FInitiator() = default;
#if !DISABLE_S3
	FInitiator(const Aws::S3::Model::Initiator& Data)
		: ID(UTF8_TO_TCHAR(Data.GetID().c_str()))
		, DisplayName(UTF8_TO_TCHAR(Data.GetDisplayName().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString DisplayName;
};

USTRUCT(BlueprintType)
struct FMultipartUpload
{
	GENERATED_BODY()

public:
	FMultipartUpload()
		: Initiated(0)
		, StorageClass(EStorageClass::NOT_SET)
	{
	}
#if !DISABLE_S3
	FMultipartUpload(const Aws::S3::Model::MultipartUpload& Data)
		: UploadId(UTF8_TO_TCHAR(Data.GetUploadId().c_str()))
		, Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, StorageClass(static_cast<EStorageClass>(Data.GetStorageClass()))
		, Owner(Data.GetOwner())
		, Initiator(Data.GetInitiator())
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetInitiated().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), Initiated);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime Initiated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInitiator Initiator;
};

USTRUCT(BlueprintType)
struct FDeletedObject
{
	GENERATED_BODY()

public:
	FDeletedObject()
		: bDeleteMarker(false)
	{
	}
#if !DISABLE_S3
	FDeletedObject(const Aws::S3::Model::DeletedObject& Data)
		: Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, bDeleteMarker(Data.GetDeleteMarker())
		, DeleteMarkerVersionId(UTF8_TO_TCHAR(Data.GetDeleteMarkerVersionId().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bDeleteMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString DeleteMarkerVersionId;
};

USTRUCT(BlueprintType)
struct FAbortMultipartUploadResult
{
	GENERATED_BODY()

public:
	FAbortMultipartUploadResult()
		: RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FAbortMultipartUploadResult(const Aws::S3::Model::AbortMultipartUploadResult& Data)
		: RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FCompleteMultipartUploadResult
{
	GENERATED_BODY()

public:
	FCompleteMultipartUploadResult()
		: ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FCompleteMultipartUploadResult(const Aws::S3::Model::CompleteMultipartUploadResult& Data)
		: Location(UTF8_TO_TCHAR(Data.GetLocation().c_str()))
		, Bucket(UTF8_TO_TCHAR(Data.GetBucket().c_str()))
		, Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, Expiration(UTF8_TO_TCHAR(Data.GetExpiration().c_str()))
		, ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, ServerSideEncryption(static_cast<EServerSideEncryption>(Data.GetServerSideEncryption()))
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, SSEKMSKeyId(UTF8_TO_TCHAR(Data.GetSSEKMSKeyId().c_str()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Location;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Expiration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FCreateBucketResult
{
	GENERATED_BODY()

public:
	FCreateBucketResult() = default;
#if !DISABLE_S3
	FCreateBucketResult(const Aws::S3::Model::CreateBucketResult& Data)
		: Location(UTF8_TO_TCHAR(Data.GetLocation().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Location;
};

USTRUCT(BlueprintType)
struct FCreateMultipartUploadResult
{
	GENERATED_BODY()

public:
	FCreateMultipartUploadResult()
		: AbortDate(0)
		, ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FCreateMultipartUploadResult(const Aws::S3::Model::CreateMultipartUploadResult& Data)
		: AbortRuleId(UTF8_TO_TCHAR(Data.GetAbortRuleId().c_str()))
		, Bucket(UTF8_TO_TCHAR(Data.GetBucket().c_str()))
		, Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, UploadId(UTF8_TO_TCHAR(Data.GetUploadId().c_str()))
		, ServerSideEncryption(static_cast<EServerSideEncryption>(Data.GetServerSideEncryption()))
		, SSECustomerAlgorithm(UTF8_TO_TCHAR(Data.GetSSECustomerAlgorithm().c_str()))
		, SSECustomerKeyMD5(UTF8_TO_TCHAR(Data.GetSSECustomerKeyMD5().c_str()))
		, SSEKMSKeyId(UTF8_TO_TCHAR(Data.GetSSEKMSKeyId().c_str()))
		, SSEKMSEncryptionContext(UTF8_TO_TCHAR(Data.GetSSEKMSEncryptionContext().c_str()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetAbortDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), AbortDate);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime AbortDate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString AbortRuleId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSEncryptionContext;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FDeleteObjectResult
{
	GENERATED_BODY()

public:
	FDeleteObjectResult()
		: bDeleteMarker(false)
		, RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FDeleteObjectResult(const Aws::S3::Model::DeleteObjectResult& Data)
		: bDeleteMarker(Data.GetDeleteMarker())
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bDeleteMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FDeleteObjectTaggingResult
{
	GENERATED_BODY()

public:
	FDeleteObjectTaggingResult() = default;
#if !DISABLE_S3
	FDeleteObjectTaggingResult(const Aws::S3::Model::DeleteObjectTaggingResult& Data)
		: VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
};

USTRUCT(BlueprintType)
struct FDeleteObjectsResult
{
	GENERATED_BODY()

public:
	FDeleteObjectsResult()
		: RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FDeleteObjectsResult(const Aws::S3::Model::DeleteObjectsResult& Data)
		: RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
		for (auto& Element : Data.GetDeleted())
			Deleted.Add(Element);

		for (auto& Element : Data.GetErrors())
			Errors.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FDeletedObject> Deleted;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Error> Errors;
};

USTRUCT(BlueprintType)
struct FGetBucketAccelerateConfigurationResult
{
	GENERATED_BODY()

public:
	FGetBucketAccelerateConfigurationResult()
		: Status(EBucketAccelerateStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FGetBucketAccelerateConfigurationResult(const Aws::S3::Model::GetBucketAccelerateConfigurationResult& Data)
		: Status(static_cast<EBucketAccelerateStatus>(Data.GetStatus()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketAccelerateStatus Status;
};

USTRUCT(BlueprintType)
struct FGetBucketAclResult
{
	GENERATED_BODY()

public:
	FGetBucketAclResult() = default;
#if !DISABLE_S3
	FGetBucketAclResult(const Aws::S3::Model::GetBucketAclResult& Data)
		: Owner(Data.GetOwner())
	{
		for (auto& Element : Data.GetGrants())
			Grants.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FGrant> Grants;
};

USTRUCT(BlueprintType)
struct FGetBucketAnalyticsConfigurationResult
{
	GENERATED_BODY()

public:
	FGetBucketAnalyticsConfigurationResult() = default;
#if !DISABLE_S3
	FGetBucketAnalyticsConfigurationResult(const Aws::S3::Model::GetBucketAnalyticsConfigurationResult& Data)
		: AnalyticsConfiguration(Data.GetAnalyticsConfiguration())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FAnalyticsConfiguration AnalyticsConfiguration;
};

USTRUCT(BlueprintType)
struct FGetBucketCorsResult
{
	GENERATED_BODY()

public:
	FGetBucketCorsResult() = default;
#if !DISABLE_S3
	FGetBucketCorsResult(const Aws::S3::Model::GetBucketCorsResult& Data)
	{
		for (auto& Element : Data.GetCORSRules())
			CORSRules.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FCORSRule> CORSRules;
};

USTRUCT(BlueprintType)
struct FGetBucketEncryptionResult
{
	GENERATED_BODY()

public:
	FGetBucketEncryptionResult() = default;
#if !DISABLE_S3
	FGetBucketEncryptionResult(const Aws::S3::Model::GetBucketEncryptionResult& Data)
		: ServerSideEncryptionConfiguration(Data.GetServerSideEncryptionConfiguration())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FServerSideEncryptionConfiguration ServerSideEncryptionConfiguration;
};

USTRUCT(BlueprintType)
struct FGetBucketInventoryConfigurationResult
{
	GENERATED_BODY()

public:
	FGetBucketInventoryConfigurationResult() = default;
#if !DISABLE_S3
	FGetBucketInventoryConfigurationResult(const Aws::S3::Model::GetBucketInventoryConfigurationResult& Data)
		: InventoryConfiguration(Data.GetInventoryConfiguration())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInventoryConfiguration InventoryConfiguration;
};

USTRUCT(BlueprintType)
struct FGetBucketLifecycleConfigurationResult
{
	GENERATED_BODY()

public:
	FGetBucketLifecycleConfigurationResult() = default;
#if !DISABLE_S3
	FGetBucketLifecycleConfigurationResult(const Aws::S3::Model::GetBucketLifecycleConfigurationResult& Data)
	{
		for (auto& Element : Data.GetRules())
			Rules.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FLifecycleRule> Rules;
};

USTRUCT(BlueprintType)
struct FGetBucketLocationResult
{
	GENERATED_BODY()

public:
	FGetBucketLocationResult()
		: LocationConstraint(EBucketLocationConstraint::NOT_SET)
	{
	}
#if !DISABLE_S3
	FGetBucketLocationResult(const Aws::S3::Model::GetBucketLocationResult& Data)
		: LocationConstraint(static_cast<EBucketLocationConstraint>(Data.GetLocationConstraint()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketLocationConstraint LocationConstraint;
};

USTRUCT(BlueprintType)
struct FGetBucketLoggingResult
{
	GENERATED_BODY()

public:
	FGetBucketLoggingResult() = default;
#if !DISABLE_S3
	FGetBucketLoggingResult(const Aws::S3::Model::GetBucketLoggingResult& Data)
		: LoggingEnabled(Data.GetLoggingEnabled())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FLoggingEnabled LoggingEnabled;
};

USTRUCT(BlueprintType)
struct FGetBucketMetricsConfigurationResult
{
	GENERATED_BODY()

public:
	FGetBucketMetricsConfigurationResult() = default;
#if !DISABLE_S3
	FGetBucketMetricsConfigurationResult(const Aws::S3::Model::GetBucketMetricsConfigurationResult& Data)
		: MetricsConfiguration(Data.GetMetricsConfiguration())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FMetricsConfiguration MetricsConfiguration;
};

USTRUCT(BlueprintType)
struct FGetBucketNotificationConfigurationResult
{
	GENERATED_BODY()

public:
	FGetBucketNotificationConfigurationResult() = default;
#if !DISABLE_S3
	FGetBucketNotificationConfigurationResult(const Aws::S3::Model::GetBucketNotificationConfigurationResult& Data)
	{
		for (auto& Element : Data.GetTopicConfigurations())
			TopicConfigurations.Add(Element);

		for (auto& Element : Data.GetQueueConfigurations())
			QueueConfigurations.Add(Element);

		for (auto& Element : Data.GetLambdaFunctionConfigurations())
			LambdaFunctionConfigurations.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FTopicConfiguration> TopicConfigurations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FQueueConfiguration> QueueConfigurations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FLambdaFunctionConfiguration> LambdaFunctionConfigurations;
};

USTRUCT(BlueprintType)
struct FGetBucketPolicyResult
{
	GENERATED_BODY()

public:
	FGetBucketPolicyResult() = default;

	FGetBucketPolicyResult(const FString& message)
		: Response(message)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Response;
};

USTRUCT(BlueprintType)
struct FGetBucketPolicyStatusResult
{
	GENERATED_BODY()

public:
	FGetBucketPolicyStatusResult() = default;
#if !DISABLE_S3
	FGetBucketPolicyStatusResult(const Aws::S3::Model::GetBucketPolicyStatusResult& Data)
		: PolicyStatus(Data.GetPolicyStatus())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FPolicyStatus PolicyStatus;
};

USTRUCT(BlueprintType)
struct FGetBucketReplicationResult
{
	GENERATED_BODY()

public:
	FGetBucketReplicationResult() = default;
#if !DISABLE_S3
	FGetBucketReplicationResult(const Aws::S3::Model::GetBucketReplicationResult& Data)
		: ReplicationConfiguration(Data.GetReplicationConfiguration())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FReplicationConfiguration ReplicationConfiguration;
};

USTRUCT(BlueprintType)
struct FGetBucketRequestPaymentResult
{
	GENERATED_BODY()

public:
	FGetBucketRequestPaymentResult()
		: Payer(EPayer::NOT_SET)
	{
	}
#if !DISABLE_S3
	FGetBucketRequestPaymentResult(const Aws::S3::Model::GetBucketRequestPaymentResult& Data)
		: Payer(static_cast<EPayer>(Data.GetPayer()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EPayer Payer;
};

USTRUCT(BlueprintType)
struct FGetBucketTaggingResult
{
	GENERATED_BODY()

public:
	FGetBucketTaggingResult() = default;
#if !DISABLE_S3
	FGetBucketTaggingResult(const Aws::S3::Model::GetBucketTaggingResult& Data)
	{
		for (auto& Element : Data.GetTagSet())
			TagSet.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Tag> TagSet;
};

USTRUCT(BlueprintType)
struct FGetBucketVersioningResult
{
	GENERATED_BODY()

public:
	FGetBucketVersioningResult()
		: Status(EBucketVersioningStatus::NOT_SET)
		, MFADelete(EMFADeleteStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FGetBucketVersioningResult(const Aws::S3::Model::GetBucketVersioningResult& Data)
		: Status(static_cast<EBucketVersioningStatus>(Data.GetStatus()))
		, MFADelete(static_cast<EMFADeleteStatus>(Data.GetMFADelete()))

	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EBucketVersioningStatus Status;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EMFADeleteStatus MFADelete;
};

USTRUCT(BlueprintType)
struct FGetBucketWebsiteResult
{
	GENERATED_BODY()

public:
	FGetBucketWebsiteResult() = default;
#if !DISABLE_S3
	FGetBucketWebsiteResult(const Aws::S3::Model::GetBucketWebsiteResult& Data)
		: RedirectAllRequestsTo(Data.GetRedirectAllRequestsTo())
		, IndexDocument(Data.GetIndexDocument())
		, ErrorDocument(Data.GetErrorDocument())
	{
		for (auto& Element : Data.GetRoutingRules())
			RoutingRules.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FRedirectAllRequestsTo RedirectAllRequestsTo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FIndexDocument IndexDocument;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FErrorDocument ErrorDocument;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FRoutingRule> RoutingRules;
};

USTRUCT(BlueprintType)
struct FGetObjectResult
{
	GENERATED_BODY()

public:
	FGetObjectResult()
		: bDeleteMarker(false)
		, LastModified(0)
		, ContentLength(0)
		, MissingMeta(0)
		, Expires(0)
		, ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, StorageClass(EStorageClass::NOT_SET)
		, RequestCharged(ERequestCharged::NOT_SET)
		, ReplicationStatus(EReplicationStatus::NOT_SET)
		, PartsCount(0)
		, TagCount(0)
		, ObjectLockMode(EObjectLockMode::NOT_SET)
		, ObjectLockRetainUntilDate(0)
		, ObjectLockLegalHoldStatus(EObjectLockLegalHoldStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FGetObjectResult(const Aws::S3::Model::GetObjectResult& Data)
		: bDeleteMarker(Data.GetDeleteMarker())
		, AcceptRanges(UTF8_TO_TCHAR(Data.GetAcceptRanges().c_str()))
		, Expiration(UTF8_TO_TCHAR(Data.GetExpiration().c_str()))
		, Restore(UTF8_TO_TCHAR(Data.GetRestore().c_str()))
		, ContentLength(Data.GetContentLength())
		, ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, MissingMeta(Data.GetMissingMeta())
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, CacheControl(UTF8_TO_TCHAR(Data.GetCacheControl().c_str()))
		, ContentDisposition(UTF8_TO_TCHAR(Data.GetContentDisposition().c_str()))
		, ContentEncoding(UTF8_TO_TCHAR(Data.GetContentEncoding().c_str()))
		, ContentLanguage(UTF8_TO_TCHAR(Data.GetContentLanguage().c_str()))
		, ContentRange(UTF8_TO_TCHAR(Data.GetContentRange().c_str()))
		, ContentType(UTF8_TO_TCHAR(Data.GetContentType().c_str()))
		, WebsiteRedirectLocation(UTF8_TO_TCHAR(Data.GetWebsiteRedirectLocation().c_str()))
		, ServerSideEncryption(static_cast<EServerSideEncryption>(Data.GetServerSideEncryption()))
		, SSECustomerAlgorithm(UTF8_TO_TCHAR(Data.GetSSECustomerAlgorithm().c_str()))
		, SSECustomerKeyMD5(UTF8_TO_TCHAR(Data.GetSSECustomerKeyMD5().c_str()))
		, SSEKMSKeyId(UTF8_TO_TCHAR(Data.GetSSEKMSKeyId().c_str()))
		, StorageClass(static_cast<EStorageClass>(Data.GetStorageClass()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
		, ReplicationStatus(static_cast<EReplicationStatus>(Data.GetReplicationStatus()))
		, PartsCount(Data.GetPartsCount())
		, TagCount(Data.GetTagCount())
		, ObjectLockMode(static_cast<EObjectLockMode>(Data.GetObjectLockMode()))
		, ObjectLockLegalHoldStatus(EObjectLockLegalHoldStatus::NOT_SET)
		, Id2(UTF8_TO_TCHAR(Data.GetId2().c_str()))
		, RequestId(UTF8_TO_TCHAR(Data.GetRequestId().c_str()))

	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastModified().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastModified);
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetExpires().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), Expires);
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetObjectLockRetainUntilDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), ObjectLockRetainUntilDate);

		for (auto& Element : Data.GetMetadata())
			Metadata.Add(UTF8_TO_TCHAR(Element.first.c_str()), UTF8_TO_TCHAR(Element.second.c_str()));
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bDeleteMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString AcceptRanges;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Expiration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Restore;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime LastModified;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int64 ContentLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MissingMeta;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CacheControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentDisposition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentEncoding;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentLanguage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime Expires;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString WebsiteRedirectLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> Metadata;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EReplicationStatus ReplicationStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartsCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 TagCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockMode ObjectLockMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime ObjectLockRetainUntilDate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockLegalHoldStatus ObjectLockLegalHoldStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Id2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString RequestId;
};

USTRUCT(BlueprintType)
struct FGetObjectAclResult
{
	GENERATED_BODY()

public:
	FGetObjectAclResult()
		: RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FGetObjectAclResult(const Aws::S3::Model::GetObjectAclResult& Data)
		: Owner(Data.GetOwner())
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
		for (auto& Element : Data.GetGrants())
			Grants.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FGrant> Grants;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FGetObjectLegalHoldResult
{
	GENERATED_BODY()

public:
	FGetObjectLegalHoldResult() = default;
#if !DISABLE_S3
	FGetObjectLegalHoldResult(const Aws::S3::Model::GetObjectLegalHoldResult& Data)
		: LegalHold(Data.GetLegalHold())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FObjectLockLegalHold LegalHold;
};

USTRUCT(BlueprintType)
struct FGetObjectLockConfigurationResult
{
	GENERATED_BODY()

public:
	FGetObjectLockConfigurationResult() = default;
#if !DISABLE_S3
	FGetObjectLockConfigurationResult(const Aws::S3::Model::GetObjectLockConfigurationResult& Data)
		: ObjectLockConfiguration(Data.GetObjectLockConfiguration())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FObjectLockConfiguration ObjectLockConfiguration;
};

USTRUCT(BlueprintType)
struct FGetObjectRetentionResult
{
	GENERATED_BODY()

public:
	FGetObjectRetentionResult() = default;
#if !DISABLE_S3
	FGetObjectRetentionResult(const Aws::S3::Model::GetObjectRetentionResult& Data)
		: Retention(Data.GetRetention())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FObjectLockRetention Retention;
};

USTRUCT(BlueprintType)
struct FGetObjectTaggingResult
{
	GENERATED_BODY()

public:
	FGetObjectTaggingResult() = default;
#if !DISABLE_S3
	FGetObjectTaggingResult(const Aws::S3::Model::GetObjectTaggingResult& Data)
		: VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
	{
		for (auto& Element : Data.GetTagSet())
			TagSet.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Tag> TagSet;
};

USTRUCT(BlueprintType)
struct FGetObjectTorrentResult
{
	GENERATED_BODY()

public:
	FGetObjectTorrentResult()
		: Body(nullptr)
		, RequestCharged(ERequestCharged::NOT_SET)
	{
	}

public:
#if !DISABLE_S3
	Aws::Utils::Stream::ResponseStream* Body;
#else
	void* Body;
#endif

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FGetPublicAccessBlockResult
{
	GENERATED_BODY()

public:
	FGetPublicAccessBlockResult() = default;
#if !DISABLE_S3
	FGetPublicAccessBlockResult(const Aws::S3::Model::GetPublicAccessBlockResult& Data)
		: PublicAccessBlockConfiguration(Data.GetPublicAccessBlockConfiguration())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FPublicAccessBlockConfiguration PublicAccessBlockConfiguration;
};

USTRUCT(BlueprintType)
struct FHeadObjectResult
{
	GENERATED_BODY()

public:
	FHeadObjectResult()
		: bDeleteMarker(false)
		, LastModified(0)
		, ContentLength(0)
		, MissingMeta(0)
		, Expires(0)
		, ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, StorageClass(EStorageClass::NOT_SET)
		, RequestCharged(ERequestCharged::NOT_SET)
		, ReplicationStatus(EReplicationStatus::NOT_SET)
		, PartsCount(0)
		, ObjectLockMode(EObjectLockMode::NOT_SET)
		, ObjectLockRetainUntilDate(0)
		, ObjectLockLegalHoldStatus(EObjectLockLegalHoldStatus::NOT_SET)
	{
	}
#if !DISABLE_S3
	FHeadObjectResult(const Aws::S3::Model::HeadObjectResult& Data)
		: bDeleteMarker(Data.GetDeleteMarker())
		, AcceptRanges(UTF8_TO_TCHAR(Data.GetAcceptRanges().c_str()))
		, Expiration(UTF8_TO_TCHAR(Data.GetExpiration().c_str()))
		, Restore(UTF8_TO_TCHAR(Data.GetRestore().c_str()))
		, ContentLength(Data.GetContentLength())
		, ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, MissingMeta(Data.GetMissingMeta())
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, CacheControl(UTF8_TO_TCHAR(Data.GetCacheControl().c_str()))
		, ContentDisposition(UTF8_TO_TCHAR(Data.GetContentDisposition().c_str()))
		, ContentEncoding(UTF8_TO_TCHAR(Data.GetContentEncoding().c_str()))
		, ContentLanguage(UTF8_TO_TCHAR(Data.GetContentLanguage().c_str()))
		, ContentType(UTF8_TO_TCHAR(Data.GetContentType().c_str()))
		, WebsiteRedirectLocation(UTF8_TO_TCHAR(Data.GetWebsiteRedirectLocation().c_str()))
		, ServerSideEncryption(static_cast<EServerSideEncryption>(Data.GetServerSideEncryption()))
		, SSECustomerAlgorithm(UTF8_TO_TCHAR(Data.GetSSECustomerAlgorithm().c_str()))
		, SSECustomerKeyMD5(UTF8_TO_TCHAR(Data.GetSSECustomerKeyMD5().c_str()))
		, SSEKMSKeyId(UTF8_TO_TCHAR(Data.GetSSEKMSKeyId().c_str()))
		, StorageClass(static_cast<EStorageClass>(Data.GetStorageClass()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
		, ReplicationStatus(static_cast<EReplicationStatus>(Data.GetReplicationStatus()))
		, PartsCount(Data.GetPartsCount())
		, ObjectLockMode(static_cast<EObjectLockMode>(Data.GetObjectLockMode()))
		, ObjectLockLegalHoldStatus(static_cast<EObjectLockLegalHoldStatus>(Data.GetObjectLockLegalHoldStatus()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetLastModified().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), LastModified);
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetExpires().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), Expires);
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetObjectLockRetainUntilDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), ObjectLockRetainUntilDate);

		for (auto& Element : Data.GetMetadata())
			Metadata.Add(UTF8_TO_TCHAR(Element.first.c_str()), UTF8_TO_TCHAR(Element.second.c_str()));
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bDeleteMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString AcceptRanges;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Expiration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Restore;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime LastModified;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int64 ContentLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MissingMeta;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CacheControl;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentDisposition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentEncoding;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentLanguage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContentType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime Expires;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString WebsiteRedirectLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TMap<FString, FString> Metadata;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EReplicationStatus ReplicationStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartsCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockMode ObjectLockMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime ObjectLockRetainUntilDate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EObjectLockLegalHoldStatus ObjectLockLegalHoldStatus;
};

USTRUCT(BlueprintType)
struct FListBucketAnalyticsConfigurationsResult
{
	GENERATED_BODY()

public:
	FListBucketAnalyticsConfigurationsResult()
		: bIsTruncated(false)
	{
	}
#if !DISABLE_S3
	FListBucketAnalyticsConfigurationsResult(const Aws::S3::Model::ListBucketAnalyticsConfigurationsResult& Data)
		: bIsTruncated(Data.GetIsTruncated())
		, ContinuationToken(UTF8_TO_TCHAR(Data.GetContinuationToken().c_str()))
		, NextContinuationToken(UTF8_TO_TCHAR(Data.GetNextContinuationToken().c_str()))
	{
		for (auto& Element : Data.GetAnalyticsConfigurationList())
			AnalyticsConfigurationList.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FAnalyticsConfiguration> AnalyticsConfigurationList;
};

USTRUCT(BlueprintType)
struct FListBucketInventoryConfigurationsResult
{
	GENERATED_BODY()

public:
	FListBucketInventoryConfigurationsResult()
		: bIsTruncated(false)
	{
	}
#if !DISABLE_S3
	FListBucketInventoryConfigurationsResult(const Aws::S3::Model::ListBucketInventoryConfigurationsResult& Data)
		: ContinuationToken(UTF8_TO_TCHAR(Data.GetContinuationToken().c_str()))
		, bIsTruncated(Data.GetIsTruncated())
		, NextContinuationToken(UTF8_TO_TCHAR(Data.GetNextContinuationToken().c_str()))
	{
		for (auto& Element : Data.GetInventoryConfigurationList())
			InventoryConfigurationList.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FInventoryConfiguration> InventoryConfigurationList;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextContinuationToken;
};

USTRUCT(BlueprintType)
struct FListBucketMetricsConfigurationsResult
{
	GENERATED_BODY()

public:
	FListBucketMetricsConfigurationsResult()
		: bIsTruncated(false)
	{
	}
#if !DISABLE_S3
	FListBucketMetricsConfigurationsResult(const Aws::S3::Model::ListBucketMetricsConfigurationsResult& Data)
		: bIsTruncated(Data.GetIsTruncated())
		, ContinuationToken(UTF8_TO_TCHAR(Data.GetContinuationToken().c_str()))
		, NextContinuationToken(UTF8_TO_TCHAR(Data.GetNextContinuationToken().c_str()))
	{
		for (auto& Element : Data.GetMetricsConfigurationList())
			MetricsConfigurationList.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FMetricsConfiguration> MetricsConfigurationList;
};

USTRUCT(BlueprintType)
struct FListBucketsResult
{
	GENERATED_BODY()

public:
	FListBucketsResult() = default;
#if !DISABLE_S3
	FListBucketsResult(const Aws::S3::Model::ListBucketsResult& Data)
		: Owner(Data.GetOwner())
	{
		for (auto& Element : Data.GetBuckets())
			Buckets.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FBucket> Buckets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
};

USTRUCT(BlueprintType)
struct FListMultipartUploadsResult
{
	GENERATED_BODY()

public:
	FListMultipartUploadsResult()
		: MaxUploads(0)
		, bIsTruncated(false)
		, EncodingType(EEncodingType::NOT_SET)
	{
	}
#if !DISABLE_S3
	FListMultipartUploadsResult(const Aws::S3::Model::ListMultipartUploadsResult& Data)
		: Bucket(UTF8_TO_TCHAR(Data.GetBucket().c_str()))
		, KeyMarker(UTF8_TO_TCHAR(Data.GetKeyMarker().c_str()))
		, UploadIdMarker(UTF8_TO_TCHAR(Data.GetUploadIdMarker().c_str()))
		, NextKeyMarker(UTF8_TO_TCHAR(Data.GetNextKeyMarker().c_str()))
		, Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Delimiter(UTF8_TO_TCHAR(Data.GetDelimiter().c_str()))
		, NextUploadIdMarker(UTF8_TO_TCHAR(Data.GetNextUploadIdMarker().c_str()))
		, MaxUploads(Data.GetMaxUploads())
		, bIsTruncated(Data.GetIsTruncated())
		, EncodingType(static_cast<EEncodingType>(Data.GetEncodingType()))
	{
		for (auto& Element : Data.GetUploads())
			Uploads.Add(Element);

		for (auto& Element : Data.GetCommonPrefixes())
			CommonPrefixes.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KeyMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadIdMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextKeyMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Delimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextUploadIdMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxUploads;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FMultipartUpload> Uploads;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FCommonPrefix> CommonPrefixes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EEncodingType EncodingType;
};

USTRUCT(BlueprintType)
struct FListObjectVersionsResult
{
	GENERATED_BODY()

public:
	FListObjectVersionsResult()
		: bIsTruncated(false)
		, MaxKeys(0)
		, EncodingType(EEncodingType::NOT_SET)
	{
	}
#if !DISABLE_S3
	FListObjectVersionsResult(const Aws::S3::Model::ListObjectVersionsResult& Data)
		: bIsTruncated(Data.GetIsTruncated())
		, KeyMarker(UTF8_TO_TCHAR(Data.GetKeyMarker().c_str()))
		, VersionIdMarker(UTF8_TO_TCHAR(Data.GetVersionIdMarker().c_str()))
		, NextKeyMarker(UTF8_TO_TCHAR(Data.GetNextKeyMarker().c_str()))
		, NextVersionIdMarker(UTF8_TO_TCHAR(Data.GetNextVersionIdMarker().c_str()))
		, Name(UTF8_TO_TCHAR(Data.GetName().c_str()))
		, Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Delimiter(UTF8_TO_TCHAR(Data.GetDelimiter().c_str()))
		, MaxKeys(Data.GetMaxKeys())
		, EncodingType(static_cast<EEncodingType>(Data.GetEncodingType()))
	{
		for (auto& Element : Data.GetVersions())
			Versions.Add(Element);

		for (auto& Element : Data.GetDeleteMarkers())
			DeleteMarkers.Add(Element);

		for (auto& Element : Data.GetCommonPrefixes())
			CommonPrefixes.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString KeyMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionIdMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextKeyMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextVersionIdMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FObjectVersion> Versions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FDeleteMarkerEntry> DeleteMarkers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Delimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxKeys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FCommonPrefix> CommonPrefixes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EEncodingType EncodingType;
};

USTRUCT(BlueprintType)
struct FListObjectsResult
{
	GENERATED_BODY()

public:
	FListObjectsResult()
		: bIsTruncated(false)
		, MaxKeys(0)
		, EncodingType(EEncodingType::NOT_SET)
	{
	}
#if !DISABLE_S3
	FListObjectsResult(const Aws::S3::Model::ListObjectsResult& Data)
		: bIsTruncated(Data.GetIsTruncated())
		, Marker(UTF8_TO_TCHAR(Data.GetMarker().c_str()))
		, NextMarker(UTF8_TO_TCHAR(Data.GetNextMarker().c_str()))
		, Name(UTF8_TO_TCHAR(Data.GetName().c_str()))
		, Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Delimiter(UTF8_TO_TCHAR(Data.GetDelimiter().c_str()))
		, MaxKeys(Data.GetMaxKeys())
		, EncodingType(static_cast<EEncodingType>(Data.GetEncodingType()))
	{
		for (auto& Element : Data.GetContents())
			Contents.Add(Element);

		for (auto& Element : Data.GetCommonPrefixes())
			CommonPrefixes.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Marker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Object> Contents;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Delimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxKeys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FCommonPrefix> CommonPrefixes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EEncodingType EncodingType;
};

USTRUCT(BlueprintType)
struct FListObjectsV2Result
{
	GENERATED_BODY()

public:
	FListObjectsV2Result()
		: bIsTruncated(false)
		, MaxKeys(0)
		, EncodingType(EEncodingType::NOT_SET)
		, KeyCount(0)
	{
	}
#if !DISABLE_S3
	FListObjectsV2Result(const Aws::S3::Model::ListObjectsV2Result& Data)
		: bIsTruncated(Data.GetIsTruncated())
		, Name(UTF8_TO_TCHAR(Data.GetName().c_str()))
		, Prefix(UTF8_TO_TCHAR(Data.GetPrefix().c_str()))
		, Delimiter(UTF8_TO_TCHAR(Data.GetDelimiter().c_str()))
		, MaxKeys(Data.GetMaxKeys())
		, EncodingType(static_cast<EEncodingType>(Data.GetEncodingType()))
		, KeyCount(Data.GetKeyCount())
		, ContinuationToken(UTF8_TO_TCHAR(Data.GetContinuationToken().c_str()))
		, NextContinuationToken(UTF8_TO_TCHAR(Data.GetNextContinuationToken().c_str()))
		, StartAfter(UTF8_TO_TCHAR(Data.GetStartAfter().c_str()))
	{
		for (auto& Element : Data.GetContents())
			Contents.Add(Element);

		for (auto& Element : Data.GetCommonPrefixes())
			CommonPrefixes.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Object> Contents;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Prefix;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Delimiter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxKeys;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FCommonPrefix> CommonPrefixes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EEncodingType EncodingType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 KeyCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString NextContinuationToken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString StartAfter;
};

USTRUCT(BlueprintType)
struct FListPartsResult
{
	GENERATED_BODY()

public:
	FListPartsResult()
		: AbortDate(0)
		, PartNumberMarker(0)
		, NextPartNumberMarker(0)
		, MaxParts(0)
		, bIsTruncated(false)
		, StorageClass(EStorageClass::NOT_SET)
		, RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FListPartsResult(const Aws::S3::Model::ListPartsResult& Data)
		: AbortRuleId(UTF8_TO_TCHAR(Data.GetAbortRuleId().c_str()))
		, Bucket(UTF8_TO_TCHAR(Data.GetBucket().c_str()))
		, Key(UTF8_TO_TCHAR(Data.GetKey().c_str()))
		, UploadId(UTF8_TO_TCHAR(Data.GetUploadId().c_str()))
		, PartNumberMarker(Data.GetPartNumberMarker())
		, NextPartNumberMarker(Data.GetNextPartNumberMarker())
		, MaxParts(Data.GetMaxParts())
		, bIsTruncated(Data.GetIsTruncated())
		, Initiator(Data.GetInitiator())
		, Owner(Data.GetOwner())
		, StorageClass(static_cast<EStorageClass>(Data.GetStorageClass()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
		FDateTime::ParseIso8601(UTF8_TO_TCHAR(Data.GetAbortDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601).c_str()), AbortDate);

		for (auto& Element : Data.GetParts())
			Parts.Add(Element);
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime AbortDate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString AbortRuleId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Bucket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString UploadId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 PartNumberMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 NextPartNumberMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int32 MaxParts;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool bIsTruncated;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	TArray<FS3Part> Parts;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FInitiator Initiator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FOwner Owner;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FPutObjectResult
{
	GENERATED_BODY()

public:
	FPutObjectResult()
		: ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FPutObjectResult(const Aws::S3::Model::PutObjectResult& Data)
		: Expiration(UTF8_TO_TCHAR(Data.GetExpiration().c_str()))
		, ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, ServerSideEncryption(static_cast<EServerSideEncryption>(Data.GetServerSideEncryption()))
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, SSECustomerAlgorithm(UTF8_TO_TCHAR(Data.GetSSECustomerAlgorithm().c_str()))
		, SSECustomerKeyMD5(UTF8_TO_TCHAR(Data.GetSSECustomerKeyMD5().c_str()))
		, SSEKMSKeyId(UTF8_TO_TCHAR(Data.GetSSEKMSKeyId().c_str()))
		, SSEKMSEncryptionContext(UTF8_TO_TCHAR(Data.GetSSEKMSEncryptionContext().c_str()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString Expiration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSEncryptionContext;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FPutObjectAclResult
{
	GENERATED_BODY()

public:
	FPutObjectAclResult()
		: RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FPutObjectAclResult(const Aws::S3::Model::PutObjectAclResult& Data)
		: RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FPutObjectLegalHoldResult
{
	GENERATED_BODY()

public:
	FPutObjectLegalHoldResult()
		: RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FPutObjectLegalHoldResult(const Aws::S3::Model::PutObjectLegalHoldResult& Data)
		: RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FPutObjectLockConfigurationResult
{
	GENERATED_BODY()

public:
	FPutObjectLockConfigurationResult()
		: RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FPutObjectLockConfigurationResult(const Aws::S3::Model::PutObjectLockConfigurationResult& Data)
		: RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FPutObjectRetentionResult
{
	GENERATED_BODY()

public:
	FPutObjectRetentionResult()
		: RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FPutObjectRetentionResult(const Aws::S3::Model::PutObjectRetentionResult& Data)
		: RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FPutObjectTaggingResult
{
	GENERATED_BODY()

public:
	FPutObjectTaggingResult() = default;
#if !DISABLE_S3
	FPutObjectTaggingResult(const Aws::S3::Model::PutObjectTaggingResult& Data)
		: VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
};

USTRUCT(BlueprintType)
struct FRestoreObjectResult
{
	GENERATED_BODY()

public:
	FRestoreObjectResult()
		: RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FRestoreObjectResult(const Aws::S3::Model::RestoreObjectResult& Data)
		: RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
		, RestoreOutputPath(UTF8_TO_TCHAR(Data.GetRestoreOutputPath().c_str()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString RestoreOutputPath;
};

USTRUCT(BlueprintType)
struct FUploadPartResult
{
	GENERATED_BODY()

public:
	FUploadPartResult()
		: ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FUploadPartResult(const Aws::S3::Model::UploadPartResult& Data)
		: ServerSideEncryption(static_cast<EServerSideEncryption>(Data.GetServerSideEncryption()))
		, ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, SSECustomerAlgorithm(UTF8_TO_TCHAR(Data.GetSSECustomerAlgorithm().c_str()))
		, SSECustomerKeyMD5(UTF8_TO_TCHAR(Data.GetSSECustomerKeyMD5().c_str()))
		, SSEKMSKeyId(UTF8_TO_TCHAR(Data.GetSSEKMSKeyId().c_str()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};

USTRUCT(BlueprintType)
struct FGetObjectAttributesResult
{
	GENERATED_BODY()

public:
	FGetObjectAttributesResult()
		: DeleteMarker(false)
		, RequestCharged(ERequestCharged::NOT_SET)
		, StorageClass(EStorageClass::NOT_SET)
		, ObjectSize(0)
	{
	}
#if !DISABLE_S3
	FGetObjectAttributesResult(const Aws::S3::Model::GetObjectAttributesResult& Data)
		: DeleteMarker(Data.GetDeleteMarker())
		, VersionId(UTF8_TO_TCHAR(Data.GetVersionId().c_str()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
		, ETag(UTF8_TO_TCHAR(Data.GetETag().c_str()))
		, Checksum(Data.GetChecksum())
		, ObjectParts(Data.GetObjectParts())
		, StorageClass(static_cast<EStorageClass>(Data.GetStorageClass()))
		, ObjectSize(Data.GetObjectSize())
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	bool DeleteMarker;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FDateTime LastModified;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString VersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString ETag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FChecksum Checksum;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FGetObjectAttributesParts ObjectParts;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EStorageClass StorageClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	int64 ObjectSize;
};


USTRUCT(BlueprintType)
struct FUploadPartCopyResult
{
	GENERATED_BODY()

public:
	FUploadPartCopyResult()
		: ServerSideEncryption(EServerSideEncryption::NOT_SET)
		, RequestCharged(ERequestCharged::NOT_SET)
	{
	}
#if !DISABLE_S3
	FUploadPartCopyResult(const Aws::S3::Model::UploadPartCopyResult& Data)
		: CopySourceVersionId(UTF8_TO_TCHAR(Data.GetCopySourceVersionId().c_str()))
		, CopyPartResult(Data.GetCopyPartResult())
		, ServerSideEncryption(static_cast<EServerSideEncryption>(Data.GetServerSideEncryption()))
		, SSECustomerAlgorithm(UTF8_TO_TCHAR(Data.GetSSECustomerAlgorithm().c_str()))
		, SSECustomerKeyMD5(UTF8_TO_TCHAR(Data.GetSSECustomerKeyMD5().c_str()))
		, SSEKMSKeyId(UTF8_TO_TCHAR(Data.GetSSEKMSKeyId().c_str()))
		, RequestCharged(static_cast<ERequestCharged>(Data.GetRequestCharged()))
	{
	}
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString CopySourceVersionId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FCopyPartResult CopyPartResult;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	EServerSideEncryption ServerSideEncryption;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerAlgorithm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSECustomerKeyMD5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	FString SSEKMSKeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AWS|S3")
	ERequestCharged RequestCharged;
};
