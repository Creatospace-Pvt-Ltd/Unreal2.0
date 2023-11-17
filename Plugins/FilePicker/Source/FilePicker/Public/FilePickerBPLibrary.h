// Copyright 2022 FBAM.Studio

#pragma once

#include "Core.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "FilePickerBPLibrary.generated.h"


UCLASS()
class UFilePickerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "FilePicker")
	static bool GetFilesAndDirectories(const FString& FullPathOfBaseDir, TArray<FString>& FilenamesOut, TArray<FString>& DirectorynamesOut);

	UFUNCTION(BlueprintCallable, Category = "FilePicker")
	static bool IsValidPath(const FString& FullPath);

	UFUNCTION(BlueprintCallable, Category = "FilePicker")
		static FString GetFileExt(const FString& FilePath);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FilePicker", meta = (WorldContext = "WorldContextObject"))
		static bool IsRunningInEditor(const UObject* WorldContextObject) {
			if (!IsValid(WorldContextObject)) return false;
			if (!IsValid(WorldContextObject->GetWorld())) return false;

			if (WorldContextObject->GetWorld()->WorldType == EWorldType::PIE
				|| WorldContextObject->GetWorld()->WorldType == EWorldType::Editor) {
				return true;
			}

			return false;
		}
};



// PlatformFileFunctor
//
// Task: Needed for Directory iteration
//
template <class FunctorType>
class PlatformFileFunctor : public IPlatformFile::FDirectoryVisitor
{
public:

	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override {
		return FileFunctor(FilenameOrDirectory, bIsDirectory);
	}

	PlatformFileFunctor(FunctorType&& FunctorInstance): FileFunctor(MoveTemp(FunctorInstance)) {}

private:
	FunctorType FileFunctor;
};
// PlatformFileFunctor



// MakeDirectoryVisitor()
//
// Task: Prepares Directory Visitor
//
template <class Functor>
PlatformFileFunctor<Functor> MakeDirectoryVisitor(Functor&& FunctorInstance)
{
	return PlatformFileFunctor<Functor>(MoveTemp(FunctorInstance));
}
// MakeDirectoryVisitor()