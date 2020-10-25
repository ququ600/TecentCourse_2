// Copyright Epic Games, Inc. All Rights Reserved.

#include "TecentCourse_2GameMode.h"
#include "TecentCourse_2Character.h"
#include "UObject/ConstructorHelpers.h"

ATecentCourse_2GameMode::ATecentCourse_2GameMode()
{

	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
