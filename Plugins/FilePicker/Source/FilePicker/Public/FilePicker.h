// Copyright 2022 FBAM.Studio

#pragma once

#include "Modules/ModuleManager.h"

class FFilePickerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
