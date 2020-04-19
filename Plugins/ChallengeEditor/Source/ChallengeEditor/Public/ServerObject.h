// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/Runnable.h"
#include "Sockets.h"
#include "TickableEditorObject.h"

class FServerObject : public FTickableEditorObject
{

private:

	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;

	class FRunnableThread* ListenerThread;

	bool HasClientConnected;

public:

	void Start();

public:

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FMockServer, STATGROUP_Tickables);
	}


private:

	bool DoOnListenAction();
	void DoOnConnectionAction();
	void DoOnSocketDisconnected();
};


class FServerWorkerThread : public FRunnable
{
public:

	FServerWorkerThread(TFunction<bool()> ListenActionCallback, TFunction<void()> ConnectionActionCallback)
	{
		IsRunning = true;
		ListenAction = ListenActionCallback;
		ConnectionAction = ConnectionActionCallback;
	}	

	virtual uint32 Run() override
	{
		while (IsRunning)
		{
			if (!ListenAction())
			{
				return 0;
			}

			ConnectionAction();

			FPlatformProcess::Sleep(1.f);
		}

		return 0;
	}

	virtual void Stop() override
	{
		
	}

	virtual void Exit() override
	{

	}

private:

	TFunction<bool()> ListenAction;
	TFunction<void()> ConnectionAction;
	bool IsRunning;
};



