// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "LNDNode.h"

class FLNDWalletPluginModule : public IModuleInterface
{
public:

	FProcHandle LndProc;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
