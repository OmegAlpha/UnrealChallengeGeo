// Fill out your copyright notice in the Description page of Project Settings.


#include "LightRotatorActor.h"
#include "Engine/Engine.h"
#include "MessageEndpoint.h"
#include "MessageEndpointBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectBase.h"
#include "Engine/DirectionalLight.h"
#include "GeoChallenge.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "ClientObject.h"

ALightRotatorActor::ALightRotatorActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALightRotatorActor::BeginPlay()
{
	Super::BeginPlay();

#if (UE_BUILD_SHIPPING)	
 	TArray<AActor*> Lights;
 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADirectionalLight::StaticClass(), Lights);
 	
 	DirectionalLight = Cast<ADirectionalLight>(Lights[0]);

  	ClientObject = NewObject<UClientObject>();
  	ClientObject->Start(GetWorld());	
#endif
}

void ALightRotatorActor::BeginDestroy()
{
	Super::BeginDestroy();
}

void ALightRotatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if (UE_BUILD_SHIPPING)	
	float SliderVal = FMath::FloorToFloat(ClientObject->GetSliderValue() * 1000.f) / 1000.f;

	FRotator Rot = DirectionalLight->GetActorRotation();

	Rot.Pitch = FMath::Lerp(-90.f, 90.f, SliderVal);

	DirectionalLight->SetActorRelativeRotation(Rot);
#endif
}