//Copyright Geodesic 2020

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FChatParticipantInfo
{
	FText ParticipantName = FText::FromString("Chat Participant");

	FChatParticipantInfo() = default;
	FChatParticipantInfo(FText NewParticipantName)
	{
		ParticipantName = NewParticipantName;
	}
};

class SGeodesicChatWindow : public SCompoundWidget, public FGeodesicChatMessagingServer, public FGeodesicChatMessagingClient
{
public:

	SLATE_BEGIN_ARGS(SGeodesicChatWindow)
	{}

	SLATE_END_ARGS()

	// Constructs this widget with InArgs. Needed for every widget. Builds this widget and any of its children
	void Construct(const FArguments& InArgs);
	void Destruct();

	void InitGeodesicChatWindow(FChatParticipantInfo NewParticipantInfo);

	FReply SendMessage();

	void HandleGeodesicChatMessage(
		const FGeodesicChatMessageInfo& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context) override;

	void CommitEditableText(const FText& InText, ETextCommit::Type InCommitType);

	// Slate Widgets as variables
	TSharedPtr<SEditableTextBox> ediChatInputText;
	TSharedPtr<SScrollBox> scrMessageList;

private:
	// Chat Information
	FChatParticipantInfo ParticipantInfo;
};