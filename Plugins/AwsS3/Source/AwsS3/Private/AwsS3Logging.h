/**
* Copyright (C) 2017-2023 eelDev AB
*/

#pragma once

#define LogDebug(format, ...) UE_LOG(LogAwsS3, Log, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVerbose(format, ...) UE_LOG(LogAwsS3, Verbose, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVeryVerbose(format, ...) UE_LOG(LogAwsS3, VeryVerbose, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) UE_LOG(LogAwsS3, Error, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) UE_LOG(LogAwsS3, Warning, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
