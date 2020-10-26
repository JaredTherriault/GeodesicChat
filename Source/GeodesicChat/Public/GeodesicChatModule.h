//Copyright Geodesic 2020

#pragma once

#include "Modules/ModuleInterface.h"
#include "SlateBasics.h"
#include "GeodesicChatWindow.h"
#include "GeodesicChatMessage.h"
#include "Styling/ISlateStyle.h"

DECLARE_LOG_CATEGORY_EXTERN(ModuleLog, Log, All);

class FGeodesicChatModule : public IModuleInterface
{
private:

public:
	FGeodesicChatModule();

	// Plugin methods
	void StartupModule() override;
	void ShutdownModule() override;

	// Debug message logging
	static void PrintToLog(FString LogMessage);
	static void PrintWarningToLog(FString LogMessage);
	static void PrintErrorToLog(FString LogMessage);

	// For creating toolbar button
	void AddToolbarButton(FToolBarBuilder& Builder);
	TSharedRef<SWidget> FillComboButton(TSharedPtr<class FUICommandList> Commands);

	// For our chat tabs
	void InitializeChatTabMembers();
	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& TabSpawnArgs);

	// Some statics to facilitate a singleton workflow
	static inline FGeodesicChatModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FGeodesicChatModule >("GeodesicChat");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("GeodesicChat");
	}

	// Command list (for combo button sub menu options)
	TSharedPtr<FUICommandList> PluginCommands;
	
	// What we set up in InitializeChatTabMembers() to identify our chat tabs
	TArray < FName > ChatTabNames;
	
	// A reference to our opened chat windows
	TArray<TSharedPtr< SGeodesicChatWindow >> ChatWindowWidgets;

	// Chat Colors
	FLinearColor FirstPersonColor = FLinearColor(0.1f, 0.1f, 1.0f, 1.0f); // Blue-ish
	FLinearColor ThirdPersonColor = FLinearColor(1.0f, 0.1f, 0.1f, 1.0f); // Red-ish
};

class FGeodesicChatStyle
{
public:

	// Statics similar to the plugin overrides, but for the style we created
	static void Initialize();
	static void Shutdown();
	
	// Get singleton
	static TSharedPtr< class ISlateStyle > Get();

	//Easier to get the name from the singleton than to write it in manually
	static FName GetStyleSetName();

private:

	// Singleton as a variable
	static TSharedPtr<class FSlateStyleSet> Style;
};

class FChatEditorCommands : public TCommands<FChatEditorCommands>
{
public:
	// Set up commands
	FChatEditorCommands()
		: TCommands<FChatEditorCommands>
		(
			TEXT("GeodesicChat"),
			NSLOCTEXT("GeodesicChat", "GeodesicChat", "GeodesicChat Plugin"),
			NAME_None,
			FGeodesicChatStyle::GetStyleSetName()
			) {}

	virtual void RegisterCommands() override;

	// Enumerate commands
	TSharedPtr<FUICommandInfo> OpenChat01;
	TSharedPtr<FUICommandInfo> OpenChat02;
};