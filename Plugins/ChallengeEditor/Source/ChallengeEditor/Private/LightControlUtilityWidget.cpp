// Fill out your copyright notice in the Description page of Project Settings.


#include "LightControlUtilityWidget.h"
#include "Components/Slider.h"
#include "WidgetBlueprint.h"
#include "Framework/Docking/TabManager.h"
#include "Editor/EditorEngine.h"
#include "Widgets/Docking/SDockTab.h"
#include "Engine/Engine.h"
#include "Editor.h"

float ULightControlUtilityWidget::SliderValue;

FName TabID = TEXT("LightPitchTool");

bool IsTabOpened = false;

SDockTab* DockTab = nullptr;

void ULightControlUtilityWidget::Open()
{
	if (IsTabOpened)
		return;

	auto bp = LoadObject<UWidgetBlueprint>(nullptr, TEXT("EditorUtilityWidgetBlueprint'/Game/eWP_LightControlPanel.eWP_LightControlPanel'"));

	if (bp != nullptr && !FGlobalTabmanager::Get()->HasTabSpawner(TabID))
	{		
		FGlobalTabmanager::Get()->RegisterTabSpawner(
			TabID,
			FOnSpawnTab::CreateLambda([&](const FSpawnTabArgs& Args)
				{
					UWorld* World = GEditor->GetEditorWorldContext().World();
					check(World);

					TSubclassOf<ULightControlUtilityWidget> WidgetClass = bp->GeneratedClass;
					ULightControlUtilityWidget* Widget = CreateWidget<ULightControlUtilityWidget>(World, WidgetClass);

					TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab);

					DockTab = &SpawnedTab.Get();

					auto OnClosed = [](TSharedRef<SDockTab>)
					{
						IsTabOpened = false;
					};

					SpawnedTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateLambda(OnClosed));

					SpawnedTab->SetContent(Widget->TakeWidget());

					IsTabOpened = true;

					return SpawnedTab;
				}));
	}	

	FGlobalTabmanager::Get()->InvokeTab(TabID);
}


void ULightControlUtilityWidget::Close()
{
	if (DockTab != nullptr)
	{
		DockTab->RequestCloseTab();
		DockTab = nullptr;
	}
}

void ULightControlUtilityWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SliderValue = SliderLightPitch->GetValue();
}