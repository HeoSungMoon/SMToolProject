// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameAssetCleaner.h"
#include "GameAssetCleanerStyle.h"
#include "GameAssetCleanerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName GameAssetCleanerTabName("GameAssetCleaner");

#define LOCTEXT_NAMESPACE "FGameAssetCleanerModule"

void FGameAssetCleanerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGameAssetCleanerStyle::Initialize();
	FGameAssetCleanerStyle::ReloadTextures();

	FGameAssetCleanerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGameAssetCleanerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGameAssetCleanerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGameAssetCleanerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GameAssetCleanerTabName, FOnSpawnTab::CreateRaw(this, &FGameAssetCleanerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FGameAssetCleanerTabTitle", "GameAssetCleaner"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGameAssetCleanerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGameAssetCleanerStyle::Shutdown();

	FGameAssetCleanerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GameAssetCleanerTabName);
}

TSharedRef<SDockTab> FGameAssetCleanerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FGameAssetCleanerModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("GameAssetCleaner.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FGameAssetCleanerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GameAssetCleanerTabName);
}

void FGameAssetCleanerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGameAssetCleanerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGameAssetCleanerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameAssetCleanerModule, GameAssetCleaner)