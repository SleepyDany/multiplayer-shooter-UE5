// Copyright Project F1. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "F1BaseCharacter.generated.h"

class UCameraComponent;
class UF1InputSettings;
class USpringArmComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(F1LogBaseCharacter, Log, All);

UCLASS()
class AF1BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AF1BaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "X - Right velocity, Y - Forward velocity, Z = 0"))
	FVector GetMovementVelocity2D() const;
	void TryJump();
	virtual void NotifyJumpApex() override;
	void Walk();
	void StartSprint();
	void StopSprint();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "F1|Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "F1|Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "F1|Movement")
	bool bStartJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "F1|Movement")
	bool bIsWalking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "F1|Movement")
	float WalkMultiplier = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "F1|Movement")
	bool bIsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "F1|Movement")
	float SprintMultiplier = 1.75f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "F1|Input")
	TObjectPtr<UF1InputSettings> InputSettings;

	float DefaultMaxSpeed = 0.0f; 
};
