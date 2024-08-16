// Copyright Epic Games, Inc. All Rights Reserved.

#include "MythGameMode.h"
#include "MythCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMythGameMode::AMythGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
