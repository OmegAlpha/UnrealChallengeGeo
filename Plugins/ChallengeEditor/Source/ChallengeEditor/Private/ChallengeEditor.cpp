// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChallengeEditor.h"
#include "MessageEndpointBuilder.h"
#include "UObject/ConstructorHelpers.h"
#include "LightControlUtilityWidget.h"
#include "Editor/EditorEngine.h"
#include "UMGEditor/Public/WidgetBlueprint.h"
#include "EditorUtilityBlueprint.h"

#define LOCTEXT_NAMESPACE "FChallengeEditorModule"

void FChallengeEditorModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("[ChallengeEditor] Adding the callbakcs"))

	ServerObject = MakeShareable(new FServerObject());
	ServerObject->Start();
}

void FChallengeEditorModule::ShutdownModule()
{	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FChallengeEditorModule, ChallengeEditor)