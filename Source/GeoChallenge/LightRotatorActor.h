// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MessageEndpoint.h"
#include "LightRotatorActor.generated.h"


 

USTRUCT()
struct FRotateLightMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Value;
};

USTRUCT()
struct FGameStartedMessage
{
	GENERATED_USTRUCT_BODY()
};


UCLASS()
class GEOCHALLENGE_API ALightRotatorActor : public AActor
{
	GENERATED_BODY()
	

private:

	UPROPERTY()
	class UClientObject* ClientObject;

	class ADirectionalLight* DirectionalLight;


public:	

	ALightRotatorActor();

protected:

	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	

	virtual void Tick(float DeltaTime) override;
};

