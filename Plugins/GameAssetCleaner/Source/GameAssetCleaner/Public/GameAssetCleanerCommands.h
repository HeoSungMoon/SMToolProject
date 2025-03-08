// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GameAssetCleanerStyle.h"

class FGameAssetCleanerCommands : public TCommands<FGameAssetCleanerCommands>
{
public:

	FGameAssetCleanerCommands()
		: TCommands<FGameAssetCleanerCommands>(TEXT("GameAssetCleaner"), NSLOCTEXT("Contexts", "GameAssetCleaner", "GameAssetCleaner Plugin"), NAME_None, FGameAssetCleanerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};