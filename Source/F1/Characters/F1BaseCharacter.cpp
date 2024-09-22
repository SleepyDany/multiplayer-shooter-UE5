// Copyright Project F1. All Rights Reserved.
#include "F1BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "F1/Input/F1InputSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
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

	if (const auto MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MovementComponent->bNotifyApex = true;
		DefaultMaxSpeed = MovementComponent->MaxWalkSpeed;
	}

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

		if (InputSettings->JumpAction)
		{
			EnhancedInputComponent->BindAction(InputSettings->JumpAction, ETriggerEvent::Triggered, this, &AF1BaseCharacter::TryJump);
		}

		if (InputSettings->WalkAction)
		{
			EnhancedInputComponent->BindAction(InputSettings->WalkAction, ETriggerEvent::Triggered, this, &AF1BaseCharacter::Walk);
		}

		if (InputSettings->SprintAction)
		{
			EnhancedInputComponent->BindAction(InputSettings->SprintAction, ETriggerEvent::Started, this, &AF1BaseCharacter::StartSprint);
			EnhancedInputComponent->BindAction(InputSettings->SprintAction, ETriggerEvent::Completed, this, &AF1BaseCharacter::StopSprint);
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

FVector AF1BaseCharacter::GetMovementVelocity2D() const
{
	const FVector Velocity = GetVelocity();
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();

	const FVector FVelocityVector = Velocity.ProjectOnTo(Forward);
	const FVector RVelocityVector = Velocity.ProjectOnTo(Right);

	const float FVelocity = FVelocityVector.Size2D() * FMath::Sign(FVelocityVector.Dot(Forward));
	const float RVelocity = RVelocityVector.Size2D() * FMath::Sign(RVelocityVector.Dot(Right));

	return {RVelocity, FVelocity, 0};
}

void AF1BaseCharacter::TryJump()
{
	const auto MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	check(MovementComponent);

	if (bStartJump || MovementComponent->IsFalling())
		return;

	MovementComponent->bNotifyApex = true;
	bStartJump = true;
}

void AF1BaseCharacter::NotifyJumpApex()
{
	bStartJump = false;

	Super::NotifyJumpApex();
}

void AF1BaseCharacter::Walk()
{
	bIsWalking = !bIsWalking;

	if (const auto MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()); MovementComponent && !bIsSprinting)
	{
		auto& CurSpeed = MovementComponent->MaxWalkSpeed;

		CurSpeed = bIsWalking ? DefaultMaxSpeed * WalkMultiplier : DefaultMaxSpeed;
	}
}

void AF1BaseCharacter::StartSprint()
{
	bIsSprinting = true;

	// TODO: Smooth speed acceleration?
	if (const auto MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MovementComponent->MaxWalkSpeed = DefaultMaxSpeed * SprintMultiplier;
	}
}

void AF1BaseCharacter::StopSprint()
{
	bIsSprinting = false;

	// TODO: Smooth speed acceleration?
	if (const auto MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		auto& CurSpeed = MovementComponent->MaxWalkSpeed;
		CurSpeed = bIsWalking ? DefaultMaxSpeed * WalkMultiplier : DefaultMaxSpeed;
	}
}
