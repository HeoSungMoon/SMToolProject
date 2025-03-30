// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperManager.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

void FSuperManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	InitCBMenuExtension();
	
}

void FSuperManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#pragma region ContentBrowserMenuExtension

void FSuperManagerModule::InitCBMenuExtension()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtends = ContentBrowserModule.GetAllPathViewContextMenuExtenders();
	// FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	// CustomCBMenuDelegate.BindRaw(this, &FSuperManagerModule::CustomCBMenuExtender);
	// ContentBrowserModuleMenuExtends.Add(CustomCBMenuDelegate);
	
	ContentBrowserModuleMenuExtends.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSuperManagerModule::CustomCBMenuExtender));
}

TSharedRef<FExtender> FSuperManagerModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		MenuExtender->AddMenuExtension(FName("Delete"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FSuperManagerModule::AddCBMenuEntry));

		FolderPathsSelected = SelectedPaths;
	}
	
	return MenuExtender;
}

void FSuperManagerModule::AddCBMenuEntry(class FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Delete Unused Assets")),
		FText::FromString(TEXT("Safely delete all unused assets under folder")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnDeleteUnusedAssetButtonClicked)
	);
}

void FSuperManagerModule::OnDeleteUnusedAssetButtonClicked()
{
	if (FolderPathsSelected.Num() > 1)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("You can only do this to one folder"));
		return;
	}

	TArray<FString> AssetPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);
	if (AssetPathNames.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No asset found under selected folder"));
		return;
	}

	EAppReturnType::Type ConfirmResult =
	DebugHeader::ShowMsgDialog(EAppMsgType::YesNo, TEXT("A total of ") + FString::FromInt(AssetPathNames.Num()) + TEXT(" found.\nWoudle you like to procceed?"));

	if (ConfirmResult == EAppReturnType::No) return;

	TArray<FAssetData> UnusedAssetsData;

	for (const FString& AssetPath : AssetPathNames)
	{
		// Don't touch root folder
		if (AssetPath.Contains(TEXT("Developers")) || AssetPath.Contains(TEXT("Collections")))
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPath)) continue;

		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPath);

		if (AssetReferencers.Num() == 0)
		{
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPath);
			UnusedAssetsData.Add(UnusedAssetData);
		}
	}

	if (UnusedAssetsData.Num() > 0)
	{
		ObjectTools::DeleteAssets(UnusedAssetsData);
	}
	else
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No unused asset found under selected folder"));
	}
}

#pragma endregion

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSuperManagerModule, SuperManager)