/**
* Copyright (C) 2017-2023 eelDev AB
*/

#include "AwsS3Library.h"
#include "AwsS3Logging.h"
#include "AwsS3PrivatePCH.h"
#include "Async/Async.h"

bool UAwsS3Library::DirectoryExists(FString Directory)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.DirectoryExists(*Directory);
}

bool UAwsS3Library::CreateDirectory(FString Directory)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.CreateDirectory(*Directory);
}

bool UAwsS3Library::DeleteDirectory(FString Directory)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.DeleteDirectory(*Directory);
}

bool UAwsS3Library::DeleteFile(FString Filename)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.DeleteFile(*Filename);
}

bool UAwsS3Library::IsReadOnly(FString Filename)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.IsReadOnly(*Filename);
}

bool UAwsS3Library::MoveFile(FString To, FString From)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.MoveFile(*To, *From);
}

bool UAwsS3Library::SetReadOnly(FString Filename, bool bNewReadOnlyValue)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	return PlatformFile.SetReadOnly(*Filename, bNewReadOnlyValue);
}

int64 UAwsS3Library::FileSizeOnDisk(FString File)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const int64 FileSizeOnDisk = PlatformFile.FileSize(*File);

	return FileSizeOnDisk;
}

FString UAwsS3Library::Int64ToString(int64 Value)
{
	return LexToString(Value);
}

void US3UUID::AwsS3Uuid(FString& UUID)
{
	LogVerbose("");
	UUID.Empty();
	
#if !DISABLE_S3
	aws_uuid Data;
	aws_uuid_init(&Data);

	uint8_t uuid_array[AWS_UUID_STR_LEN] = {};
	aws_byte_buf uuid_buffer = aws_byte_buf_from_array(uuid_array, sizeof(uuid_array));
	uuid_buffer.len = 0;

	aws_uuid_to_str(&Data, &uuid_buffer);
	
	UUID = UTF8_TO_TCHAR(uuid_buffer.buffer);
#endif
}

void UAwsS3ClientObject::Activate()
{
	LogVerbose("");
#if !DISABLE_S3
	
	m_Future = Async(EAsyncExecution::Thread, [this]()
	{
		LogVerbose("Creating AwsS3Client...");
		m_ClientConfigurationObject = new Aws::Client::ClientConfiguration;
		m_CredentialsObject = new Aws::Auth::AWSCredentials;

		if (!m_ClientConfiguration.UserAgent.IsEmpty())
		{
			m_ClientConfigurationObject->userAgent = TCHAR_TO_UTF8(*m_ClientConfiguration.UserAgent);
		}
		m_ClientConfigurationObject->scheme = static_cast<Aws::Http::Scheme>(m_ClientConfiguration.Sceheme);
		if (m_ClientConfiguration.Region.IsEmpty() == false)
		{
			m_ClientConfigurationObject->region = TCHAR_TO_UTF8(*m_ClientConfiguration.Region);
		}
		m_ClientConfigurationObject->useDualStack = m_ClientConfiguration.bUseDualStack;
		m_ClientConfigurationObject->maxConnections = m_ClientConfiguration.MaxConnections;
		m_ClientConfigurationObject->httpRequestTimeoutMs = m_ClientConfiguration.HTTP_RequestTimeoutMs;
		m_ClientConfigurationObject->requestTimeoutMs = m_ClientConfiguration.RequestTimeoutMs;
		m_ClientConfigurationObject->connectTimeoutMs = m_ClientConfiguration.ConnectTimeoutMs;
		m_ClientConfigurationObject->enableTcpKeepAlive = m_ClientConfiguration.bEnableTcpKeepAlive;
		m_ClientConfigurationObject->tcpKeepAliveIntervalMs = m_ClientConfiguration.TcpKeepAliveIntervalMs;
		m_ClientConfigurationObject->lowSpeedLimit = m_ClientConfiguration.LowSpeedLimit;

		if (m_ClientConfiguration.EndPointOverride.IsEmpty() == false)
		{
			m_ClientConfigurationObject->endpointOverride = TCHAR_TO_UTF8(*m_ClientConfiguration.EndPointOverride);
		}
		m_ClientConfigurationObject->proxyScheme = static_cast<Aws::Http::Scheme>(m_ClientConfiguration.ProxyScheme);

		if (m_ClientConfiguration.ProxyHost.IsEmpty() == false)
		{
			m_ClientConfigurationObject->proxyHost = TCHAR_TO_UTF8(*m_ClientConfiguration.ProxyHost);
		}
		m_ClientConfigurationObject->proxyPort = m_ClientConfiguration.ProxyPort;

		if (m_ClientConfiguration.ProxyUsername.IsEmpty() == false)
		{
			m_ClientConfigurationObject->proxyUserName = TCHAR_TO_UTF8(*m_ClientConfiguration.ProxyUsername);
		}
		if (m_ClientConfiguration.ProxyPassword.IsEmpty() == false)
		{
			m_ClientConfigurationObject->proxyPassword = TCHAR_TO_UTF8(*m_ClientConfiguration.ProxyPassword);
		}
		if (m_ClientConfiguration.ProxySSL_CertPath.IsEmpty() == false)
		{
			m_ClientConfigurationObject->proxySSLCertPath = TCHAR_TO_UTF8(*m_ClientConfiguration.ProxySSL_CertPath);
		}
		if (m_ClientConfiguration.ProxySSL_CertType.IsEmpty() == false)
		{
			m_ClientConfigurationObject->proxySSLCertType = TCHAR_TO_UTF8(*m_ClientConfiguration.ProxySSL_CertType);
		}
		if (m_ClientConfiguration.ProxySSL_KeyPath.IsEmpty() == false)
		{
			m_ClientConfigurationObject->proxySSLKeyPath = TCHAR_TO_UTF8(*m_ClientConfiguration.ProxySSL_KeyPath);
		}
		if (m_ClientConfiguration.ProxySSL_KeyType.IsEmpty() == false)
		{
			m_ClientConfigurationObject->proxySSLKeyType = TCHAR_TO_UTF8(*m_ClientConfiguration.ProxySSL_KeyType);
		}
		if (m_ClientConfiguration.ProxySSL_KeyPassword.IsEmpty() == false)
		{
			m_ClientConfigurationObject->proxySSLKeyPassword = TCHAR_TO_UTF8(*m_ClientConfiguration.ProxySSL_KeyPassword);
		}
		m_ClientConfigurationObject->verifySSL = m_ClientConfiguration.bVerifySSL;
		if (m_ClientConfiguration.CaPath.IsEmpty() == false)
		{
			m_ClientConfigurationObject->caPath = TCHAR_TO_UTF8(*m_ClientConfiguration.CaPath);
		}
		if (m_ClientConfiguration.CaFile.IsEmpty() == false)
		{
			m_ClientConfigurationObject->caFile = TCHAR_TO_UTF8(*m_ClientConfiguration.CaFile);
		}

		m_ClientConfigurationObject->followRedirects = static_cast<Aws::Client::FollowRedirectsPolicy>(m_ClientConfiguration.FollowRedirects);
		m_ClientConfigurationObject->disableExpectHeader = m_ClientConfiguration.bDisableExpectHeader;
		m_ClientConfigurationObject->enableClockSkewAdjustment = m_ClientConfiguration.bEnableClockSkewAdjustment;
		m_ClientConfigurationObject->enableHostPrefixInjection = m_ClientConfiguration.bEnableHostPrefixInjection;
		m_ClientConfigurationObject->enableEndpointDiscovery = m_ClientConfiguration.bEnableEndpointDiscovery;

		if (m_ClientConfiguration.ProfileName.IsEmpty() == false)
		{
			m_ClientConfigurationObject->profileName = TCHAR_TO_UTF8(*m_ClientConfiguration.ProfileName);
		}
		
		m_ClientConfigurationObject->executor = Aws::MakeShared<Aws::Utils::Threading::PooledThreadExecutor>("S3", 10);

		if (m_Credentials.AccessKeyId.IsEmpty() == false)
		{
			m_CredentialsObject->SetAWSAccessKeyId(TCHAR_TO_UTF8(*m_Credentials.AccessKeyId));
		}
		if (m_Credentials.SecretKey.IsEmpty() == false)
		{
			m_CredentialsObject->SetAWSSecretKey(TCHAR_TO_UTF8(*m_Credentials.SecretKey));
		}
		if (m_Credentials.SessionToken.IsEmpty() == false)
		{
			m_CredentialsObject->SetSessionToken(TCHAR_TO_UTF8(*m_Credentials.SessionToken));
		}
		if (m_Credentials.Expiration.GetTicks() > 0)
		{
			m_CredentialsObject->SetExpiration(Aws::Utils::DateTime(TCHAR_TO_UTF8(*m_Credentials.Expiration.ToIso8601()), Aws::Utils::DateFormat::ISO_8601));
		}

		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			m_S3ClientObject = new Aws::S3::S3Client(*m_CredentialsObject, *m_ClientConfigurationObject, static_cast<Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy>(m_PayloadSigningPolicy), false);

			m_Callback.ExecuteIfBound(this);
			LogVerbose("AwsS3Client Created!");
		});
	});

#endif
}

void UAwsS3ClientObject::CreateAwsS3Object(FAwsS3Credentials Credentials, FAwsS3ClientConfiguration ClientConfiguration, EPayloadSigningPolicy PayloadSigningPolicy, const FOnS3Callback& Callback)
{
	LogVerbose("");

	UAwsS3ClientObject* Proxy = NewObject<UAwsS3ClientObject>();
	Proxy->AddToRoot();
	Proxy->m_Credentials = Credentials;
	Proxy->m_ClientConfiguration = ClientConfiguration;
	Proxy->m_PayloadSigningPolicy = PayloadSigningPolicy;
	Proxy->m_Callback = Callback;

	Proxy->Activate();
}

void UAwsS3ClientObject::DestroyS3Object()
{
	LogVerbose("Object (%s) marked for deletion", *this->GetName());

	RemoveFromRoot();
}
