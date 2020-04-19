// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MessageEndpoint.h"
#include "LightControlUtilityWidget.generated.h"


UCLASS()
class CHALLENGEEDITOR_API ULightControlUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
private:

	static float SliderValue;
	
public:

	static void Open();
	static void Close();
	static float GetSliderValue() { return SliderValue; }

private:

protected:

	UPROPERTY(meta = (BindWidget))
	class USlider* SliderLightPitch;	

public :

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
