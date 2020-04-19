// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientObject.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Common/TcpSocketBuilder.h"
#include "HAL/RunnableThread.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "IPAddress.h"
#include "Containers/UnrealString.h"

void UClientObject::Start(UWorld* World)
{
	Close();

	ConnectionSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TcpIpClient-LightsTool"), false);

	if (ConnectionSocket == nullptr) return;

	int32 Size;
	ConnectionSocket->SetReceiveBufferSize(1024*1024, Size);

	WorldContext = World;
	WorldContext->GetTimerManager().SetTimer(ConnectionTimerHandle, this, &UClientObject::OnConnectionTimerTick, 0.03f, true);
}


void UClientObject::Close()
{
 	if (ConnectionSocket)
 		ConnectionSocket->Close();

	ConnectionSocket = nullptr;
}

void UClientObject::BeginDestroy()
{
	Super::BeginDestroy();

	Close();
}

bool IsConnected = false;

void UClientObject::OnConnectionTimerTick()
{
	if (ConnectionSocket == nullptr)
		return;

	if (!IsConnected)
	{
		TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		Address->SetIp(FIPv4Address(127, 0, 0, 1).Value);
		Address->SetPort(13131);

		IsConnected = ConnectionSocket->Connect(*Address);		
	}

	if (IsConnected)
	{
		uint32 Size;
		
		TArray<uint8> BinaryData;

		while (ConnectionSocket->HasPendingData(Size))
		{
			TArray<uint8> TempBinaryData;
			TempBinaryData.Init(0, Size);
			
			int32 SizeReceived = 0;
			ConnectionSocket->Recv(TempBinaryData.GetData(), TempBinaryData.Num(), SizeReceived);

			BinaryData.Append(TempBinaryData);
		}

		if (BinaryData.Num() > 0)
		{
			BinaryData.Add(0);

			FString Message = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryData.GetData())));

			TArray<FString> MsgParts;
			Message.ParseIntoArray(MsgParts, TEXT("|"), true);

			if (MsgParts.Num() > 0)
			{
				SliderValue = FCString::Atof(*MsgParts.Last());
			}
		}
	}	
}