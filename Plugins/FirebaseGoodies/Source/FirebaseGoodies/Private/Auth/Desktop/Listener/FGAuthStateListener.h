// Copyright (c) 2021 Nineva Studios

#pragma once

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
#include "firebase/auth.h"

#include "Async/Async.h"

#include "Auth/FGAuthLibrary.h"

class FGAuthStateListener : public firebase::auth::AuthStateListener
{
public:
	void OnAuthStateChanged(firebase::auth::Auth* auth) override
	{
		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			UFGAuthLibrary::AuthStateChangedCallback.ExecuteIfBound();
		});
	}
};
#endif
