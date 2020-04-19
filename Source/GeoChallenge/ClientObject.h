// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sockets.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "HAL/Runnable.h"
#include "Engine/EngineTypes.h"
#include "ClientObject.generated.h"

/**
 * 
 */
UCLASS()
class GEOCHALLENGE_API UClientObject : public UObject
{
	GENERATED_BODY()

private:

    FTimerHandle ConnectionTimerHandle;

    class UWorld* WorldContext;

    FSocket* ConnectionSocket;

	bool IsConnected;

    float SliderValue;

public:

    void Start(UWorld* World);
    void Close();

    float GetSliderValue() const { return SliderValue; }

    void virtual BeginDestroy() override;

private:

    void OnConnectionTimerTick();	
};

