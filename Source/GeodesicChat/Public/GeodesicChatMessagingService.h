//Copyright Geodesic 2020

#pragma once

#include "CoreMinimal.h"
#include "IMessageContext.h"
#include "MessageEndpoint.h"
#include "MessageEndpointBuilder.h"
#include "GeodesicChatMessagingService.generated.h"

// Messaging payload struct
 USTRUCT()
struct FGeodesicChatMessageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Messaging")
		FText TimeStampAsText;

	UPROPERTY(EditAnywhere, Category = "Messaging")
		FText Author;

	UPROPERTY(EditAnywhere, Category = "Messaging")
		FText Message;

	// default constructor for the receiver
	FGeodesicChatMessageInfo() = default;
	// helper constructor for the sender
	FGeodesicChatMessageInfo(const FText TheAuthor, const FText TheMessage)
	{
		Author = TheAuthor;
		Message = TheMessage;

		// Build Timestamp
		TimeStampAsText = FText::AsTime(FDateTime::Now(), EDateTimeStyle::Default, FText::GetInvariantTimeZone());
	}
};

class FGeodesicChatMessagingServer
{
protected:
	virtual ~FGeodesicChatMessagingServer() = 0;
	// initialize the Endpoint
	virtual void InitGeodesicChatMessagingServer();
	// destroy the Endpoint
	virtual void ShutdownGeodesicChatMessagingServer();
	// handle FGeodesicChatMessageInfo
	virtual void HandleGeodesicChatMessage(
		const FGeodesicChatMessageInfo& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> TestEndpoint;
};

class FGeodesicChatMessagingClient
{
protected:
	virtual ~FGeodesicChatMessagingClient() = 0;
	// initialize the Endpoint
	virtual void InitGeodesicChatMessagingClient();
	// destroy the Endpoint
	virtual void ShutdownGeodesicChatMessagingClient();
	// trigger FTestMessage
	virtual void SendGeodesicChatMessage(const FText TheAuthor, const FText TheMessage);

	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> TestEndpoint;
};