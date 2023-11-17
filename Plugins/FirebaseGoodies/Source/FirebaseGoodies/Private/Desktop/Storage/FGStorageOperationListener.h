// Copyright (c) 2021 Nineva Studios

#pragma once

#include "CloudStorage/Callbacks/FGCloudStorageUploadCallback.h"
#include "firebase/storage.h"

class FGStorageOperationListener : public firebase::storage::Listener {
public:

    
    UFGCloudStorageUploadCallback* UploadCallback;

    FGStorageOperationListener(UFGCloudStorageUploadCallback* Callback)
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
