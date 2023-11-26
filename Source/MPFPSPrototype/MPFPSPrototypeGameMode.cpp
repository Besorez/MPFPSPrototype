// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPFPSPrototypeGameMode.h"
#include "MPFPSPrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMPFPSPrototypeGameMode::AMPFPSPrototypeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
