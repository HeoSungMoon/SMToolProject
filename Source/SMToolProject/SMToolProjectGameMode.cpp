// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMToolProjectGameMode.h"
#include "SMToolProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASMToolProjectGameMode::ASMToolProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
