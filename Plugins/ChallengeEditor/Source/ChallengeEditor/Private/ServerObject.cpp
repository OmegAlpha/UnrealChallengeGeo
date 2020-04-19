// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerObject.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Class.h"
#include "Common/TcpSocketBuilder.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"
#include "HAL/RunnableThread.h"
#include "EditorUtilityBlueprint.h"
#include "LightControlUtilityWidget.h"


void FServerObject::Start()
{	
	auto socket = FTcpSocketBuilder(TEXT("TcpIpServer-LightsTool"))
		.AsNonBlocking()
		.BoundToPort(13131)
		.Listening(1)
		.Build();


	if (socket == nullptr)
		return;

	ListenerSocket = socket;

	ListenerThread = FRunnableThread::Create(
		new FServerWorkerThread(
			[this] { return DoOnListenAction(); },
			[this] { return DoOnConnectionAction(); }
			),
		TEXT("Light Tool Server - Listener Thread")
		);
}

void FServerObject::Tick(float DeltaTime)
{
	if (HasClientConnected)
	{
		ULightControlUtilityWidget::Open();
	}	
	else
	{
		ULightControlUtilityWidget::Close();
	}
}

bool FServerObject::DoOnListenAction()
{
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending = false;

	if (ListenerSocket->HasPendingConnection(Pending))
	{
		if (Pending)
		{
			if (ConnectionSocket != nullptr)
			{
				ConnectionSocket->Close();
				ConnectionSocket = nullptr;
			}

			ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("Received Socket Connection"));

			if (ConnectionSocket != nullptr)
			{
				HasClientConnected = true;
				ConnectionSocket->SetRecvErr(true);
			}
		}		
	}


	return true;
}

void FServerObject::DoOnConnectionAction()
{
	if (!HasClientConnected)
		return;


	// it's unlikely the state condition will work (looks like Unreal has a bug around this, that's why we also check
	// for disconnection in a condition below (when we try to send data to the client)
	if (ConnectionSocket == nullptr || ConnectionSocket->GetConnectionState() != ESocketConnectionState::SCS_Connected)
	{
		DoOnSocketDisconnected();
		return;
	}

	// We could cache the last sent message and only sent data if the value actually changed, but since 
	// we need to do a ping anyways to check if the socket has disconnected...

	FString Message = FString::SanitizeFloat(ULightControlUtilityWidget::GetSliderValue(), 4);

	// we'll use this separator in the client to filter messages and only use the latest
	Message += "|";

	TCHAR* SerializedMsg = Message.GetCharArray().GetData();
	int32 Size = FCString::Strlen(SerializedMsg);
	int Sent = 0;

	if (!ConnectionSocket->Send((uint8 *) TCHAR_TO_UTF8(SerializedMsg), Size, Sent))
	{
		DoOnSocketDisconnected();
	}
}

void FServerObject::DoOnSocketDisconnected()
{
	if (ConnectionSocket != nullptr)
	{
		ConnectionSocket->Close();
		ConnectionSocket = nullptr;
		HasClientConnected = false;
	}
}