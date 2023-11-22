// Copyright (c) 2023 King Wai Mark


#include "HTTPRequestClient.h"
#include <tchar.h>


int UHTTPRequestClient::RequestTimeoutDuration = 60;


FString UHTTPRequestClient::MethodToString(EMethod Method)
{
	switch (Method)
	{
	case EMethod::GET:
		return "GET";
	case EMethod::PUT:
		return "PUT";
	case EMethod::POST:
		return "POST";
	case EMethod::PATCH:
		return "PATCH";
	case EMethod::DEL:
		return "DELETE";
	case EMethod::COPY:
		return "COPY";
	case EMethod::HEAD:
		return "HEAD";
	case EMethod::OPTIONS:
		return "OPTIONS";
	case EMethod::LINK:
		return "LINK";
	case EMethod::UNLINK:
		return "UNLINK";
	case EMethod::LOCK:
		return "LOCK";
	case EMethod::UNLOCK:
		return "UNLOCK";
	case EMethod::PROPFIND:
		return "PROPFIND";
	case EMethod::VIEW:
		return "VIEW";
	}
	return "";
}


void UHTTPRequestClient::MakeAHttpRequest(const EMethod Method, const FString URL, const TMap<FString, FString> Params, const TMap<FString, FString> Headers, const FString Body, const FResponse &OnComplete)
{

	
	FHttpModule& HttpModule = FHttpModule::Get();

	// 1. Create Request Object
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();
	// 2. Set the request method. E.g. GET, POST, PUSH, etc.
	const FString RequestMethod = UHTTPRequestClient::MethodToString(Method);
	if(RequestMethod.Len() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Request method not set."));
		return;
	}
	HttpRequest->SetVerb(RequestMethod);

	
	// 3. Set the url.
	if(URL.Len() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The URL for the request cannot be empty."));
		return;
	};
	FString FullUrl = URL;
	// 3.a Add the query parameters using the Param map.
	int ParamIndex = 0;
	for (auto Param : Params)
	{
		if(ParamIndex == 0) FullUrl = FullUrl + "?";
		else FullUrl = FullUrl + "&";
		FullUrl = FullUrl + Param.Key + "=" + Param.Value;
		ParamIndex = ParamIndex + 1;
	}
	HttpRequest->SetURL(FullUrl);
	
	// 4. Set the request headers
	for (auto Header : Headers)
	{
		HttpRequest->AppendToHeader(Header.Key, Header.Value);
	}

	// 5. Add body to the request.
	HttpRequest->SetContentAsString(Body);

	
	// 6. Attach the callback to notify us that the process is completed.
	HttpRequest->OnProcessRequestComplete().BindLambda(
		// Here, we "capture" the 'this' pointer (the "&"), so our lambda can call this
		// class's methods in the callback.
		[&, OnComplete](
			FHttpRequestPtr Req,
			FHttpResponsePtr Res,
			bool ConnectedSuccessfully) mutable {
			OnComplete.ExecuteIfBound(Res->GetResponseCode(), Res->GetContentAsString());
			UE_LOG(LogTemp, Error, TEXT("HTTP\n %s"), *Res->GetContentAsString());
	});

	// 7. Set Settings
	HttpRequest->SetTimeout(UHTTPRequestClient::RequestTimeoutDuration);
	
	// 8. Submit the request for processing
	HttpRequest->ProcessRequest();
}

void UHTTPRequestClient::MakeAHttpRequestwithFile(const EMethod Method, const FString URL, const TMap<FString, FString> Params, const TMap<FString, FString> Headers, const FString Body, const FString FieldName, const FString FilePath, const FResponse& OnComplete)
{


	FHttpModule& HttpModule = FHttpModule::Get();

	// 1. Create Request Object
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();
	// 2. Set the request method. E.g. GET, POST, PUSH, etc.
	const FString RequestMethod = UHTTPRequestClient::MethodToString(Method);
	if (RequestMethod.Len() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Request method not set."));
		return;
	}
	HttpRequest->SetVerb(RequestMethod);


	// 3. Set the url.
	if (URL.Len() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The URL for the request cannot be empty."));
		return;
	};
	FString FullUrl = URL;
	// 3.a Add the query parameters using the Param map.
	int ParamIndex = 0;
	for (auto Param : Params)
	{
		if (ParamIndex == 0) FullUrl = FullUrl + "?";
		else FullUrl = FullUrl + "&";
		FullUrl = FullUrl + Param.Key + "=" + Param.Value;
		ParamIndex = ParamIndex + 1;
	}
	HttpRequest->SetURL(FullUrl);

	// 4. Set the request headers
	for (auto Header : Headers)
	{
		HttpRequest->AppendToHeader(Header.Key, Header.Value);
	}

	// 5. Add body to the request.
	HttpRequest->SetContentAsString(Body);

	// Add the file into the request (From Unreal forum's answer)

	// TexturePath contains the local file full path

	// file name
	int32 LastSlashPos;
	FilePath.FindLastChar('/', LastSlashPos);
	FString FileName = FilePath.RightChop(LastSlashPos + 1);

	// get data
	TArray<uint8> UpFileRawData;
	FFileHelper::LoadFileToArray(UpFileRawData, *FilePath);

	// prepare json data
	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonString);
	FString FileString;
	TSharedRef<TJsonWriter<TCHAR>> FileWriter = TJsonWriterFactory<TCHAR>::Create(&FileString);

	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("path", FilePath);
	JsonWriter->WriteValue("fieldname", FieldName);
	JsonWriter->WriteValue("imgData", UpFileRawData);
	JsonWriter->WriteValue("originalname", FileName);
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
	
	FileWriter->WriteObjectStart();
	FileWriter->WriteValue("file", JsonString);
	FileWriter->WriteObjectEnd();
	FileWriter->Close();

	// the json request
	// TSharedRef<IHttpRequest> SendJsonRequest = (&FHttpModule::Get())->CreateRequest();
	// SendJsonRequest->OnProcessRequestComplete().BindRaw(this, &SYagOpenFileWidget::HttpUploadYagFileComplete);
	// SendJsonRequest->SetURL("http://myserver/MyJsonHandlingPhpScript.php");
	// SendJsonRequest->SetVerb("POST");
	HttpRequest->SetContentAsString(FileString);
	
	
	// SendJsonRequest->ProcessRequest();


	// 6. Attach the callback to notify us that the process is completed.
	HttpRequest->OnProcessRequestComplete().BindLambda(
		// Here, we "capture" the 'this' pointer (the "&"), so our lambda can call this
		// class's methods in the callback.
		[&, OnComplete](
			FHttpRequestPtr Req,
			FHttpResponsePtr Res,
			bool ConnectedSuccessfully) mutable {
				OnComplete.ExecuteIfBound(Res->GetResponseCode(), Res->GetContentAsString());
				UE_LOG(LogTemp, Error, TEXT("HTTP\n %s"), *Res->GetContentAsString());
		});

	// 7. Set Settings
	HttpRequest->SetTimeout(UHTTPRequestClient::RequestTimeoutDuration);

	// 8. Submit the request for processing
	HttpRequest->ProcessRequest();
}

FString UHTTPRequestClient::FileToByteArray(const FString FilePath)
{
	TArray<uint8> UpFileRawData;
	FFileHelper::LoadFileToArray(UpFileRawData, *FilePath);
	// file name
	int32 LastSlashPos;
	FilePath.FindLastChar('/', LastSlashPos);
	const FString FileName = FilePath.RightChop(LastSlashPos + 1);
	int32 ExtPos;
	FileName.FindLastChar('.', ExtPos);
	const FString FileNameExact = FileName.LeftChop(FileName.Len() - 4);
	
	UE_LOG(LogTemp, Error, TEXT("Final File Name"), *FileNameExact);
	FString tempfile;

	const TCHAR* TempName = *FileNameExact;
	IFileManager &filemang = IFileManager::Get();

	bool filestatus = FFileHelper::SaveArrayToFile(UpFileRawData, TempName, &filemang, EFileWrite::FILEWRITE_AllowRead);

	
	

	if (filestatus) {
		tempfile = filemang.GetFilenameOnDisk(TempName);
		UE_LOG(LogTemp, Error, TEXT("File Created!"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("File Couldn't be Created!"));
	}

	return tempfile;
}

FJSONObject UHTTPRequestClient::ResponseStringToJSON(const FString ResponseString)
{
	FJSONObject JSONObject = FJSONObject();
	const TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseString);
	TSharedPtr<FJsonObject> JsonParsed;
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		JSONObject.JsonParsed = JsonParsed;
	}
	
	return JSONObject;
}

FJSONObject UHTTPRequestClient::GetObjectFromJSONObject(const FJSONObject JSON, const FString Key)
{
	FJSONObject JsonObject = FJSONObject();
	if(JSON.JsonParsed != nullptr)
	{
		
		JsonObject.JsonParsed = JSON.JsonParsed->GetObjectField(Key);
	} 
	return JsonObject;
}



TArray<FJSONObject> UHTTPRequestClient::GetObjectArrayFromJSONObject(const FJSONObject JSON, const FString Key)
{
	TArray<FJSONObject> ObjectArray = TArray<FJSONObject>();
	if(JSON.JsonParsed != nullptr)
	{
		for (auto ArrayField : JSON.JsonParsed->GetArrayField(Key))
		{
			FJSONObject TempJsonObject = FJSONObject();
			TempJsonObject.JsonParsed = ArrayField->AsObject();
			ObjectArray.Add(TempJsonObject);
		}
	} 
	return ObjectArray;
}

TArray<FString> UHTTPRequestClient::GetStringArrayFromJSONObject(const FJSONObject JSON, const FString Key)
{
	TArray<FString> StringArray = TArray<FString>();
	if(JSON.JsonParsed != nullptr)
	{
		
		JSON.JsonParsed->TryGetStringArrayField(Key, StringArray);
	} 
	return StringArray;
}

TArray<float> UHTTPRequestClient::GetNumberArrayFromJSONObject(const FJSONObject JSON, const FString Key)
{
	TArray<float> NumberArray = TArray<float>();
	if(JSON.JsonParsed != nullptr)
	{
		
		for (auto ArrayField : JSON.JsonParsed->GetArrayField(Key))
		{
			float TempNumber = ArrayField->AsNumber();
			NumberArray.Add(TempNumber);
		}
	} 
	return NumberArray;
}

TArray<bool> UHTTPRequestClient::GetBooleanArrayFromJSONObject(const FJSONObject JSON, const FString Key)
{
	TArray<bool> BooleanArray = TArray<bool>();
	if(JSON.JsonParsed != nullptr)
	{
		
		for (auto ArrayField : JSON.JsonParsed->GetArrayField(Key))
		{
			bool TempBool = ArrayField->AsBool();
			BooleanArray.Add(TempBool);
		}
	} 
	return BooleanArray;
}

FString UHTTPRequestClient::GetStringFromJSONObject(const FJSONObject JSON, const FString Key)
{
	FString StringData = "";
	if(JSON.JsonParsed != nullptr)
	{
		JSON.JsonParsed->TryGetStringField(Key, StringData);
	} 
	return StringData;
}

float UHTTPRequestClient::GetNumberFromJSONObject(const FJSONObject JSON, const FString Key)
{
	double NumberData = 0;
	if(JSON.JsonParsed != nullptr)
	{
		JSON.JsonParsed->TryGetNumberField(Key, NumberData);
	} 
	return NumberData;
}

bool UHTTPRequestClient::GetBooleanFromJSONObject(const FJSONObject JSON, const FString Key)
{
	bool BooleanData = false;
	if(JSON.JsonParsed != nullptr)
	{
		JSON.JsonParsed->TryGetBoolField(Key, BooleanData);
	} 
	return BooleanData;
}

void UHTTPRequestClient::SetTimeoutDuration(const int TimeoutDuration)
{
	if(RequestTimeoutDuration < 3)
	{
		UHTTPRequestClient::RequestTimeoutDuration = 3;
	} else
	{
		UHTTPRequestClient::RequestTimeoutDuration = TimeoutDuration;
	}
}

int UHTTPRequestClient::GetTimeoutDuration()
{
	return UHTTPRequestClient::RequestTimeoutDuration;
}

FString UHTTPRequestClient::Conv_JsonObjectToString(const FJSONObject& JSON)
{
	FString Result;
	if (JSON.JsonParsed.IsValid())
	{
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Result, 0);
		FJsonSerializer::Serialize(JSON.JsonParsed.ToSharedRef(), JsonWriter);
	}
	return Result;
}

FJSONObject UHTTPRequestClient::Conv_StringToJsonObject(const FString& JsonString)
{
	FJSONObject Object;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	FJsonSerializer::Deserialize(Reader, Object.JsonParsed);
	return Object;
}
