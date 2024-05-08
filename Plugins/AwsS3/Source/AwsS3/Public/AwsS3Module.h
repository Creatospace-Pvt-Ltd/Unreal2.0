/**
* Copyright (C) 2017-2023 eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "AwsS3Types.h"

#if !DISABLE_S3
class S3MemoryManagerWrapper : public Aws::Utils::Memory::MemorySystemInterface
{
public:
	virtual void* AllocateMemory(std::size_t blockSize, std::size_t alignment, const char* allocationTag = nullptr) override;
	virtual void FreeMemory(void* memoryPtr) override;
	virtual void Begin() override {};
	virtual void End() override {};
};
#endif

class AWSS3_API FAwsS3Module : public IModuleInterface
{
public:
	FAwsS3Module();
	virtual ~FAwsS3Module() override;
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void* m_sdkOptions{ nullptr };

#if !DISABLE_S3
	S3MemoryManagerWrapper m_memoryManager;
#endif
};
