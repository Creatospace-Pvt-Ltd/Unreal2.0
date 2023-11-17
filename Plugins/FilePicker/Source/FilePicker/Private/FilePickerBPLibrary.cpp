// Copyright 2022 FBAM.Studio

#include "FilePickerBPLibrary.h"
#include "FilePicker.h"


// Constructor()
//
// Task:
//
UFilePickerBPLibrary::UFilePickerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}
// Constructor()



// GetFilesAndDirectories()
//
// Task: Iterate through directories and files and return them in seperate arrays
//
bool UFilePickerBPLibrary::GetFilesAndDirectories(const FString& FullPathOfBaseDir, TArray<FString>& FilenamesOut, TArray<FString>& DirectorynamesOut)
{
	FString Str;
	auto FilenamesVisitor = MakeDirectoryVisitor(
		[&](const TCHAR* FilenameOrDirectory, bool bIsDirectory)
	{
		//Files
		if (!bIsDirectory)
		{
			Str = FPaths::GetCleanFilename(FilenameOrDirectory);
			FilenamesOut.Push(Str);
		}

		// Folders
		else {
			Str = FPaths::GetCleanFilename(FilenameOrDirectory);
			DirectorynamesOut.Push(Str);
		}

		return true;
	});

	return FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*FullPathOfBaseDir, FilenamesVisitor);
}
// GetFilesAndDirectories()



// IsValidPath()
//
// Task: check if the given path is valid
//
bool UFilePickerBPLibrary::IsValidPath(const FString& FullPath)
{
	return FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*FullPath);
}
// IsValidPath() 



// GetFileExt()
//
// Task: Return the Extension for the given File
//
FString UFilePickerBPLibrary::GetFileExt(const FString& FilePath) {
	FString Str = FPaths::GetCleanFilename(FilePath);
	return FPaths::GetExtension(Str).ToLower();
}
// GetFileExt()