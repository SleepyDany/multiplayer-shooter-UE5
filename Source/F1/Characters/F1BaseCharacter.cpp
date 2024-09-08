// Copyright Project F1. All Rights Reserved.
#include "F1BaseCharacter.h"

#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY(F1LogBaseCharacter);

AF1BaseCharacter::AF1BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(RootComponent);
}

void AF1BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}
