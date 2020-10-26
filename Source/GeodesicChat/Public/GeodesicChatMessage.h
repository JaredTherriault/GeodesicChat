//Copyright Geodesic 2020

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGeodesicChatMessage : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGeodesicChatMessage)
	{}

	SLATE_END_ARGS()

	// Constructs this widget with InArgs. Needed for every widget. Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);

	void InitializeMessage(FText TimeStampAsText, FText Author, FLinearColor UsernameColor, FText Message);

protected:
	
	TSharedPtr<STextBlock> txtAuthor;
	TSharedPtr<STextBlock> txtMessage;
};