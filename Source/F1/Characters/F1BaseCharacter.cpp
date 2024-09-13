// Copyright Project F1. All Rights Reserved.
#include "F1BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "F1/Input/F1InputSettings.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(F1LogBaseCharacter);

AF1BaseCharacter::AF1BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AF1BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const TObjectPtr<APlayerController> PlayerController = GetController<APlayerController>())
	{
		if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputSubsystem->ClearAllMappings();

			if (InputSettings && InputSettings->DefaultMappingContext)
			{
				EnhancedInputSubsystem->AddMappingContext(InputSettings->DefaultMappingContext, 0);
			}
		}
	}
}

void AF1BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);

	if (InputSettings)
	{
		if (InputSettings->LookAction)
		{
			EnhancedInputComponent->BindAction(InputSettings->LookAction, ETriggerEvent::Triggered, this, &AF1BaseCharacter::Look);
		}

		if (InputSettings->MoveAction)
		{
			EnhancedInputComponent->BindAction(InputSettings->MoveAction, ETriggerEvent::Triggered, this, &AF1BaseCharacter::Move);
		}
	}
}

void AF1BaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookDirection = Value.Get<FVector2D>();

	AddControllerPitchInput(LookDirection.Y);
	AddControllerYawInput(LookDirection.X);
}

void AF1BaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveDirection = Value.Get<FVector2D>();

	// TODO: More simple example
	// const FVector Forward = GetActorForwardVector();
	// AddMovementInput(Forward, MoveDirection.Y);
	//
	// const FVector Right = GetActorRightVector();
	// AddMovementInput(Right, MoveDirection.X);

	// TODO: Complex example
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MoveDirection.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MoveDirection.X);
}
