// Copyright (c) 2022 Nineva Studios

#include "FGFirestoreCallbacks.h"
#include "Async/Async.h"
#include "Firestore/FGDocumentSnapshot.h"
#include "Firestore/FGQuerySnapshot.h"
#include "Firestore/FGDocumentReference.h"

#if PLATFORM_ANDROID
#include "Android/Utils/FGFirebaseAndroidScopedObject.h"
#endif

UFGDocumentCallback::UFGDocumentCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGDocumentCallback::BindDelegate(const FFirestoreDocumentDelegate& InDelegate)
{
	Delegate = InDelegate;
}

void UFGDocumentCallback::ExecuteDelegate(UFGDocumentReference* Document)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		Delegate.ExecuteIfBound(Document);
	});

	RemoveFromRoot();
}

#if PLATFORM_ANDROID
void UFGDocumentCallback::ExecuteDelegateAndroid(jobject Document)
{
	AsyncTask(ENamedThreads::GameThread, [Data = NewGlobalJavaObject(Document), this]()
	{
		UFGDocumentReference* Result = NewObject<UFGDocumentReference>();
		Result->Init(*Data);
		Delegate.ExecuteIfBound(Result);
	});

	RemoveFromRoot();
}
#endif

UFGDocumentSnapshotCallback::UFGDocumentSnapshotCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGDocumentSnapshotCallback::BindDelegate(const FFirestoreDocumentSnapshotDelegate& InDelegate)
{
	Delegate = InDelegate;
}

void UFGDocumentSnapshotCallback::ExecuteDelegate(UFGDocumentSnapshot* DocumentSnapshot)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		Delegate.ExecuteIfBound(DocumentSnapshot);
	});

	RemoveFromRoot();
}

#if PLATFORM_ANDROID
void UFGDocumentSnapshotCallback::ExecuteDelegateAndroid(jobject DocumentSnapshot)
{
	AsyncTask(ENamedThreads::GameThread, [Data = NewGlobalJavaObject(DocumentSnapshot), this]()
	{
		UFGDocumentSnapshot* Snapshot = NewObject<UFGDocumentSnapshot>();
		Snapshot->Init(*Data);
		Delegate.ExecuteIfBound(Snapshot);
	});

	RemoveFromRoot();
}
#endif


UFGQuerySnapshotCallback::UFGQuerySnapshotCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGQuerySnapshotCallback::BindDelegate(const FFirestoreQuerySnapshotDelegate& InDelegate)
{
	Delegate = InDelegate;
}

void UFGQuerySnapshotCallback::ExecuteDelegate(UFGQuerySnapshot* QuerySnapshot)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		Delegate.ExecuteIfBound(QuerySnapshot);
	});

	RemoveFromRoot();
}

#if PLATFORM_ANDROID
void UFGQuerySnapshotCallback::ExecuteDelegateAndroid(jobject QuerySnapshot)
{
	AsyncTask(ENamedThreads::GameThread, [Data = NewGlobalJavaObject(QuerySnapshot), this]()
	{
		UFGQuerySnapshot* Snapshot = NewObject<UFGQuerySnapshot>();
		Snapshot->Init(*Data);
		Delegate.ExecuteIfBound(Snapshot);
	});
	
	RemoveFromRoot();
}
#endif

UFGFirestoreStringCallback::UFGFirestoreStringCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGFirestoreStringCallback::BindDelegate(const FFirestoreStringDelegate& InDelegate)
{
	Delegate = InDelegate;
}

void UFGFirestoreStringCallback::ExecuteDelegate(const FString& String)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		Delegate.ExecuteIfBound(String);
	});

	RemoveFromRoot();
}

UFGFirestoreVoidCallback::UFGFirestoreVoidCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGFirestoreVoidCallback::BindDelegate(const FFirestoreVoidDelegate& InDelegate)
{
	Delegate = InDelegate;
}

void UFGFirestoreVoidCallback::ExecuteDelegate()
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		Delegate.ExecuteIfBound();
	});

	RemoveFromRoot();
}
