// Copyright (c) 2021 Nineva Studios

#pragma once

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
#include "firebase/auth.h"

#include "Async/Async.h"

#include "Auth/FGAuthLibrary.h"

class FGIdTokenListener : public firebase::auth::IdTokenListener
{
public:
	void OnIdTokenChanged(firebase::auth::Auth* auth) override
	{
		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			UFGAuthLibrary::IdTokenChangedCallback.ExecuteIfBound();
		});
	}
};
#endif
