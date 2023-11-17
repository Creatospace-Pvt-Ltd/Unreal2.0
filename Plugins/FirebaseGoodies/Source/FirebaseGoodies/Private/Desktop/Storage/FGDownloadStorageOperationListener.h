// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudStorage/Callbacks/FGCloudStorageUploadCallback.h"
#include "firebase/storage.h"

class FGDownloadStorageOperationListener : public firebase::storage::Listener {
public:

    
    UFGCloudStorageProgressCallback* UploadCallback;

    FGDownloadStorageOperationListener(UFGCloudStorageProgressCallback* Callback)
    {
        UploadCallback = Callback;
    }
    
    void OnProgress(firebase::storage::Controller* controller) override {
        int progress = (int) ((float) controller->bytes_transferred() / (float) controller->total_byte_count() * 100);
        UploadCallback->ExecuteProgress(progress);
    }

    void OnPaused(firebase::storage::Controller* controller) override {
    }
};
