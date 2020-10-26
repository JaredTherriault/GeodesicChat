//Copyright Geodesic 2020

#include "GeodesicChatMessagingService.h"


FGeodesicChatMessagingServer::~FGeodesicChatMessagingServer()
{

}

void FGeodesicChatMessagingServer::InitGeodesicChatMessagingServer()
{
	// connect to the message bus. this will return nullptr when something fails.
	TestEndpoint = FMessageEndpoint::Builder("ReceivingEndpoint")
		// while we're at it, attach a handler for FTestMessage. this can be repeated for multiple message types.
		.Handling<FGeodesicChatMessageInfo>(this, &FGeodesicChatMessagingServer::HandleGeodesicChatMessage);
	if (TestEndpoint.IsValid())
		// start the actual subscription. without it, we won't receive messages sent by Publish() in our handling method.
		TestEndpoint->Subscribe<FGeodesicChatMessageInfo>();
}
void FGeodesicChatMessagingServer::ShutdownGeodesicChatMessagingServer()
{
	// disconnect from the message bus and stop listening for messages.
	TestEndpoint.Reset();
}

void FGeodesicChatMessagingServer::HandleGeodesicChatMessage(
	const FGeodesicChatMessageInfo& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	UE_LOG(LogTemp, Warning, TEXT("We just received a test message that said: %s"), *Message.Message.ToString());
}


FGeodesicChatMessagingClient::~FGeodesicChatMessagingClient()
{

}

void FGeodesicChatMessagingClient::InitGeodesicChatMessagingClient()
{
	// connect to the message bus. this will return nullptr when something fails.
	TestEndpoint = FMessageEndpoint::Builder("SendingEndpoint");// .Build();
	// TODO: appropriate error handling if something goes wrong :)
}
void FGeodesicChatMessagingClient::ShutdownGeodesicChatMessagingClient()
{
	// disconnect from the message bus to clean up.
	TestEndpoint.Reset();
}

void FGeodesicChatMessagingClient::SendGeodesicChatMessage(const FText TheAuthor, const FText TheMessage)
{
	if (TestEndpoint.IsValid())
		TestEndpoint->Publish(new FGeodesicChatMessageInfo(TheAuthor, TheMessage));
}