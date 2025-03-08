// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameAssetCleanerCommands.h"

#define LOCTEXT_NAMESPACE "FGameAssetCleanerModule"

void FGameAssetCleanerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "GameAssetCleaner", "Bring up GameAssetCleaner window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
