//Copyright Geodesic 2020

#include "GeodesicChatModule.h"
#include "LevelEditor.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "SlateOptMacros.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FGeodesicChatModule, GeodesicChat);

DEFINE_LOG_CATEGORY(ModuleLog)

TSharedPtr< FSlateStyleSet > FGeodesicChatStyle::Style = nullptr;
TSharedPtr< class ISlateStyle > FGeodesicChatStyle::Get() { return Style; }

FGeodesicChatModule::FGeodesicChatModule()
{

}

void FGeodesicChatModule::StartupModule()
{
	// Startup LOG message
	UE_LOG(ModuleLog, Log, TEXT("Log Started"));

	// Register commands and style (for images)
	FChatEditorCommands::Register();
	FGeodesicChatStyle::Initialize();

	// Map the commands we registered to C++ actions
	// todo: set this up with arrays to reflect the full friends list rather than hardcoding it
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FChatEditorCommands::Get().OpenChat01,
		FExecuteAction::CreateLambda([]() 
			{
				TSharedRef<class FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
				TabManager->InvokeTab(FGeodesicChatModule::Get().ChatTabNames[0]);
				PrintToLog("Launched Chat Window 01");

				FSlateApplication::Get().SetKeyboardFocus(FGeodesicChatModule::Get().ChatWindowWidgets[0]->ediChatInputText.ToSharedRef());
			})
	);
	PluginCommands->MapAction(
		FChatEditorCommands::Get().OpenChat02,
		FExecuteAction::CreateLambda([]() 
			{
				TSharedRef<class FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
				TabManager->InvokeTab(FGeodesicChatModule::Get().ChatTabNames[1]);
				PrintToLog("Launched Chat Window 02");

				FSlateApplication::Get().SetKeyboardFocus(FGeodesicChatModule::Get().ChatWindowWidgets[1]->ediChatInputText.ToSharedRef());
			})
	);

	// Get the Level Editor so we can insert our combo button to select chat windows
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	// Create a toolbar extension instance that will insert our toolbar button after the 'Settings' horizontal box in the toolbar
	TSharedPtr<FExtender> NewToolbarExtender = MakeShareable(new FExtender);
	NewToolbarExtender->AddToolBarExtension("Settings",
		EExtensionHook::After,
		PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FGeodesicChatModule::AddToolbarButton));
	// Now insert the button into the main toolbar using the extension
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(NewToolbarExtender);

	// Fill out some variables for our chat tabs
	InitializeChatTabMembers();

	// Create spawners for our chat tabs then register them
	TSharedRef<class FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
	// Create Tab Icon 
	// todo: set this up with arrays for everyone on friends list, hardcoded for now
	FSlateIcon ChatIcon = FSlateIcon(FGeodesicChatStyle::GetStyleSetName(), "GeodesicChat.ChatIcon.Small");
	TabManager->RegisterTabSpawner(ChatTabNames[0], FOnSpawnTab::CreateRaw(this, &FGeodesicChatModule::SpawnTab))
		.SetDisplayName(FText::FromName(ChatTabNames[1]))
		.SetIcon(ChatIcon);
	TabManager->RegisterTabSpawner(ChatTabNames[1], FOnSpawnTab::CreateRaw(this, &FGeodesicChatModule::SpawnTab))
		.SetDisplayName(FText::FromName(ChatTabNames[0]))
		.SetIcon(ChatIcon);
}

void FGeodesicChatModule::ShutdownModule()
{
	// Cleanup 

	// Unregister commands and Style
	FChatEditorCommands::Unregister();
	FGeodesicChatStyle::Shutdown();

	// Unregister tab spawners
	TSharedRef<class FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();

	for (int32 i = 0; i < ChatTabNames.Num(); i++)
	{
		TabManager->UnregisterTabSpawner(ChatTabNames[i]);
	}

	//Shutdown LOG message
	PrintToLog("Log Ended");
}

void FGeodesicChatModule::InitializeChatTabMembers()
{
	FGeodesicChatModule::Get().ChatTabNames.Add(FName("GeoDennis"));
	FGeodesicChatModule::Get().ChatTabNames.Add(FName("DesiKris"));
}

void FGeodesicChatModule::AddToolbarButton(FToolBarBuilder& Builder)
{
	// This creates the "Chat" combo button
	FUIAction Temp;

	Builder.AddComboButton(
		Temp,
		FOnGetContent::CreateRaw(this, &FGeodesicChatModule::FillComboButton, PluginCommands), // Add combo button subcommands menu
		FText::FromString("Chat"), // Set Text under image
		FText::FromString("Open chat window picker"), //  Set tooltip
		FSlateIcon(FGeodesicChatStyle::GetStyleSetName(), "GeodesicChat.OpenChatPicker") // Set image
	);

}

TSharedRef<SWidget> FGeodesicChatModule::FillComboButton(TSharedPtr<class FUICommandList> Commands)
{
	// Create FMenuBuilder instance for the commands we created
	FMenuBuilder MenuBuilder(true, Commands);

	// Then use it to add entries to the submenu of the combo button
	MenuBuilder.AddMenuEntry(FChatEditorCommands::Get().OpenChat01);
	MenuBuilder.AddMenuEntry(FChatEditorCommands::Get().OpenChat02);

	// Create the widget so it can be attached to the combo button
	return MenuBuilder.MakeWidget();
}

TSharedRef<SDockTab> FGeodesicChatModule::SpawnTab(const FSpawnTabArgs& TabSpawnArgs)
{
	TSharedPtr< SGeodesicChatWindow > NewChatWindow;
	// This is the actual slate code for the chat tabs. Formatting maintained manually.
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).TabRole(MajorTab) // Dock it to major tabs only, no sub-tabs
		[
			SAssignNew(NewChatWindow, SGeodesicChatWindow)
		];

	ChatWindowWidgets.Add(NewChatWindow);

	if (ChatWindowWidgets.Num() > 0)
	{
		NewChatWindow->InitGeodesicChatWindow(FChatParticipantInfo(FText::FromName(ChatTabNames[ChatWindowWidgets.Num() - 1])));
	}

	return SpawnedTab;
}

void FGeodesicChatModule::PrintToLog(FString LogMessage)
{
	LogMessage = "GeodesicChatModule: " + LogMessage;
	UE_LOG(ModuleLog, Log, TEXT("%s"), *LogMessage);
}

void FGeodesicChatModule::PrintWarningToLog(FString LogMessage)
{
	LogMessage = "GeodesicChatModule: " + LogMessage;
	UE_LOG(ModuleLog, Warning, TEXT("%s"), *LogMessage);
}

void FGeodesicChatModule::PrintErrorToLog(FString LogMessage)
{
	LogMessage = "GeodesicChatModule: " + LogMessage;
	UE_LOG(ModuleLog, Error, TEXT("%s"), *LogMessage);
}

// Allow us to use 'UI_COMMAND' macro
#define LOCTEXT_NAMESPACE "FGeodesicChatModule"

void FChatEditorCommands::RegisterCommands()
{
	// Matches menu commands to the command list
	UI_COMMAND(OpenChat01, "Chat 01", "Open the chat window (01)", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(OpenChat02, "Chat 02", "Open the chat window (02)", EUserInterfaceActionType::Button, FInputGesture());
}

// Clean up namespace declared earlier
#undef LOCTEXT_NAMESPACE

// Define a macro for ease of use when adding new image sets to our style
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

// Define predetermined image sizes (ease of use)
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon128x128(128.0f, 128.0f);

// Ask compiler to explicitly not optimize the code below for fear of spending too much time trying to optimize the code (future-proofing)
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void FGeodesicChatStyle::Initialize()
{
	if (Style.IsValid())
	{
		return;
	}

	// If we don't already have a valid style singleton then let's make one
	FGeodesicChatModule::PrintToLog("Init FGeodesicChatStyle");

	Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));

	// Set up the Resources folder as a repo for images used in the plugin 
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("GeodesicChat")->GetBaseDir() + "/Resources");

	// Define some image sets at specific sizes and apply IDs to them so they can be referenced later
	Style->Set("GeodesicChat.OpenChatPicker", new IMAGE_BRUSH(TEXT("Icon128"), Icon40x40));
	Style->Set("GeodesicChat.ChatIcon.Small", new IMAGE_BRUSH(TEXT("icon_chat"), Icon20x20));

	FSlateStyleRegistry::RegisterSlateStyle(*Style.Get());
}

// We're allowing the compiler to return to normal optimization activity
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

// Release the memory holding the macro we defined earlier since we won't be using it anymore
#undef IMAGE_BRUSH

void FGeodesicChatStyle::Shutdown()
{
	// Unregister the style we created
	if (Style.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*Style.Get());
		ensure(Style.IsUnique());
		Style.Reset();
	}
}

FName FGeodesicChatStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("GeodesicChatStyle"));
	return StyleSetName;
}
