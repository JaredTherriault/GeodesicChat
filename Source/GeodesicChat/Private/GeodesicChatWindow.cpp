//Copyright Geodesic 2020

#include "GeodesicChatWindow.h"

void SGeodesicChatWindow::Construct(const FArguments& InArgs)
{
	ChildSlot.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox) // Vertical box to hold everything
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder) // A border for style
			.BorderBackgroundColor(FColor(192, 192, 192, 255))
			.Padding(15.0f)
			[
				SAssignNew(scrMessageList, SScrollBox) // Scrollbox holds the messages
				+ SScrollBox::Slot()
				.Padding(5)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("---This the beginning of your chat history---")))
				]
			]
		]
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Bottom).AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			[
				SAssignNew(ediChatInputText, SEditableTextBox)
				.Padding(5)
				.OnTextCommitted(this, &SGeodesicChatWindow::CommitEditableText)
				.HintText(FText::FromString(TEXT("Enter a message to send...")))
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right).AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(70)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(FText::FromString(TEXT("Send")))
					.OnClicked_Raw(this, &SGeodesicChatWindow::SendMessage)
				]
			]
		]
	];

	InitGeodesicChatMessagingClient();
	InitGeodesicChatMessagingServer();
}

void SGeodesicChatWindow::Destruct()
{
	ShutdownGeodesicChatMessagingClient();
	ShutdownGeodesicChatMessagingServer();
}

void SGeodesicChatWindow::InitGeodesicChatWindow(FChatParticipantInfo NewParticipantInfo)
{
	ParticipantInfo = NewParticipantInfo;
}

FReply SGeodesicChatWindow::SendMessage()
{
	FText EnteredText = ediChatInputText->GetText();

	//We don't want to send empty messages so first let's check to see if it has content
	if (!EnteredText.IsEmptyOrWhitespace())
	{
		// First, we clear the editable text box
		ediChatInputText->SetText(FText::GetEmpty());
	
		// Use the messaging service to send the message
		SendGeodesicChatMessage(ParticipantInfo.ParticipantName, EnteredText);
	}

	FSlateApplication::Get().SetKeyboardFocus(ediChatInputText.ToSharedRef());

	return FReply::Handled();
}

void SGeodesicChatWindow::HandleGeodesicChatMessage(
	const FGeodesicChatMessageInfo& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context)
{
	// When we receive a message we need to create a new message widget
	TSharedRef<SGeodesicChatMessage> NewMessage = SNew(SGeodesicChatMessage);

	// Get the appropriate color for the time stamp and the author
	FLinearColor UsernameColor = (Message.Author.EqualTo(
		ParticipantInfo.ParticipantName) ? FGeodesicChatModule::Get().FirstPersonColor : FGeodesicChatModule::Get().ThirdPersonColor);

	// Then pass in the correct data
	NewMessage->InitializeMessage(Message.TimeStampAsText, Message.Author, UsernameColor, Message.Message);

	// And add the new widget to the scrollbox
	scrMessageList->AddSlot()[NewMessage];
}

void SGeodesicChatWindow::CommitEditableText(const FText& InText, ETextCommit::Type InCommitType)
{
	//FGeodesicChatModule::Get().PrintToLog(FString::FromInt(InCommitType));

	switch (InCommitType)
	{
	case ETextCommit::OnEnter:
		SendMessage();
		break;

	case ETextCommit::OnCleared:
		FSlateApplication::Get().SetKeyboardFocus(ediChatInputText.ToSharedRef());
		break;

	default:
		break;
	}
}
