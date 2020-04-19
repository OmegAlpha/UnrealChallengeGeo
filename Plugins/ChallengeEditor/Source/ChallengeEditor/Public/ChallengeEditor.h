// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GeoChallenge/LightRotatorActor.h"
#include "Modules/ModuleManager.h"
#include "ServerObject.h"
#include "TickableEditorObject.h"

class FChallengeEditorModule : public IModuleInterface
{

private:

 	
	TSharedPtr<FServerObject> ServerObject;

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
