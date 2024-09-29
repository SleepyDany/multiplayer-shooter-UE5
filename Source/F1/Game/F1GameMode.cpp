// Copyright Project F1. All Rights Reserved.
#include "F1GameMode.h"

#include "F1/Characters/F1BaseCharacter.h"
#include "F1/Characters/Player/F1PlayerController.h"

AF1GameMode::AF1GameMode()
{
	DefaultPawnClass = AF1BaseCharacter::StaticClass();
	PlayerControllerClass = AF1PlayerController::StaticClass();
}
