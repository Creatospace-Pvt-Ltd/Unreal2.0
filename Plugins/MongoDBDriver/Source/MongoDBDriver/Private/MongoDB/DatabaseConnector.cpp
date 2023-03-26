// 2022 Copyright Pandores Marketplace

#include "MongoDB/DatabaseConnector.h"

FDatabaseData::FDatabaseData()
	: Name()
	, SizeOnDisk(0.f)
	, bHasAnyData(false)
{}

FDatabaseData::FDatabaseData(const FDatabaseData& Other)
	: Name(Other.Name)
	, SizeOnDisk(Other.SizeOnDisk)
	, bHasAnyData(Other.bHasAnyData)
{}

FDatabaseData::FDatabaseData(FDatabaseData&& Other)
	: Name(MoveTemp(Other.Name))
	, SizeOnDisk(Other.SizeOnDisk)
	, bHasAnyData(Other.bHasAnyData)
{}

FDatabaseData& FDatabaseData::operator=(const FDatabaseData& Other)
{
	Name = Other.Name;
	SizeOnDisk = Other.SizeOnDisk;
	bHasAnyData = Other.bHasAnyData;
	
	return *this;
}

FDatabaseData& FDatabaseData::operator=(FDatabaseData&& Other)
{
	Name = MoveTemp(Other.Name);
	SizeOnDisk = Other.SizeOnDisk;
	bHasAnyData = Other.bHasAnyData;

	return *this;
}


FMongoFindOptions::FMongoFindOptions() = default;

FMongoFindOptions::FMongoFindOptions(const FMongoFindOptions& Other)
{
	*this = Other;
}

FMongoFindOptions::FMongoFindOptions(FMongoFindOptions&& Other)
{
	*this = MoveTemp(Other);
}

FMongoFindOptions& FMongoFindOptions::operator=(const FMongoFindOptions& Other)
{
	bAllowPartialResults = Other.bAllowPartialResults;
	BatchSize = Other.BatchSize;
	Collation = (Other.Collation);
	Comment = (Other.Comment);
	Limit = Other.Limit;
	Max = (Other.Max);
	Min = (Other.Min);
	MaxAwaitTime = Other.MaxAwaitTime;
	MaxTime = Other.MaxTime;
	bNoCursorTimeout = Other.bNoCursorTimeout;
	Projection = (Other.Projection);
	bReturnKey = Other.bReturnKey;
	bShowRecordId = Other.bShowRecordId;
	Skip = Other.Skip;
	Sort = (Other.Sort);

	return *this;
}

FMongoFindOptions& FMongoFindOptions::operator=(FMongoFindOptions&& Other)
{
	bAllowPartialResults = Other.bAllowPartialResults;
	BatchSize = Other.BatchSize;
	Collation = MoveTemp(Other.Collation);
	Comment = MoveTemp(Other.Comment);
	Limit = Other.Limit;
	Max = MoveTemp(Other.Max);
	Min = MoveTemp(Other.Min);
	MaxAwaitTime = Other.MaxAwaitTime;
	MaxTime = Other.MaxTime;
	bNoCursorTimeout = Other.bNoCursorTimeout;
	Projection = MoveTemp(Other.Projection);
	bReturnKey = Other.bReturnKey;
	bShowRecordId = Other.bShowRecordId;
	Skip = Other.Skip;
	Sort = MoveTemp(Other.Sort);

	return *this;
}


FDatabaseIndex::FDatabaseIndex()
	: ID(-1)
{}

FDatabaseIndex::FDatabaseIndex(const FDatabaseIndex& Other)
	: ID(Other.ID)
	, Name(Other.Name)
{}

FDatabaseIndex::FDatabaseIndex(FDatabaseIndex&& Other)
	: ID(Other.ID)
	, Name(MoveTemp(Other.Name))
{}

FDatabaseIndex& FDatabaseIndex::operator=(const FDatabaseIndex& Other)
{
	ID = Other.ID;
	Name = Other.Name;
	return *this;
}

FDatabaseIndex& FDatabaseIndex::operator=(FDatabaseIndex&& Other)
{
	ID = Other.ID;
	Name = MoveTemp(Other.Name);
	return *this;
}

// Can't use initializer list because of Clang.
static FDocumentValue CreatePingCommand()
{
	FDocumentValueMap Map;

	Map.Add(TEXT("ping"), 1);

	return FDocumentValue(Map);
}

void IDatabaseConnector::Ping(FString DatabaseName, FMongoCallback Callback)
{
	static FDocumentValue Command = CreatePingCommand();

	RunCommand(MoveTemp(DatabaseName), Command,
		FMongoDocumentCallback::CreateLambda([Callback = MoveTemp(Callback)](bool bSuccess, const FDocumentValue&) -> void
	{
		Callback.ExecuteIfBound(bSuccess);
	}));
}


