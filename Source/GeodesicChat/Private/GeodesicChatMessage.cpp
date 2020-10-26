//Copyright Geodesic 2020

#include "GeodesicChatMessage.h"
#include "GenericPlatform/GenericPlatformMisc.h"

void SGeodesicChatMessage::Construct(const FArguments& InArgs)
{
	FSlateFontInfo Bold;
	ChildSlot.HAlign(HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().AutoWidth()
		[
			SAssignNew(txtAuthor,STextBlock)
			.Text(FText::FromString(TEXT("Username: ")))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 10))
		]
		+ SHorizontalBox::Slot()
		[
			SAssignNew(txtMessage,STextBlock)
			.AutoWrapText(true)
			.Text(FText::FromString(TEXT("My message goes here")))
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10))
		]
	];
}

void SGeodesicChatMessage::InitializeMessage(FText TimeStampAsText, FText Username, FLinearColor UsernameColor, FText Message)
{
	// Update text in widget
	txtAuthor->SetText(FText::Format(FText::FromString("[{0}] {1}: "), TimeStampAsText, Username));
	txtAuthor->SetColorAndOpacity(UsernameColor);
	txtMessage->SetText(Message);
}
