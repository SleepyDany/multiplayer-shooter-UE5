// Copyright Project F1. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "F1BaseCharacter.generated.h"

class UTextRenderComponent;
class UF1HealthComponent;
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
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Look(const FInputActionValue& Value);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "F1|Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "F1|Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	void Move(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "X - Right velocity, Y - Forward velocity, Z = 0"))
	FVector GetMovementVelocity2D() const;
	void TryJump();
	virtual void NotifyJumpApex() override;
	UFUNCTION()
	void OnLandedHandler(const FHitResult& HitResult);
	void Walk();
	void StartSprint();
	void StopSprint();
	
	UPROPERTY(BlueprintReadWrite, Category = "F1|Movement")
	bool bStartJump = false;
	UPROPERTY(EditDefaultsOnly, Category = "F1|Damage|Movement")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1500.0f);
	UPROPERTY(EditDefaultsOnly, Category = "F1|Damage|Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(BlueprintReadWrite, Category = "F1|Movement")
	bool bIsWalking = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Movement")
	float WalkMultiplier = 0.5f;
	UPROPERTY(BlueprintReadWrite, Category = "F1|Movement")
	bool bIsSprinting = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Movement")
	float SprintMultiplier = 1.75f;
	
	UFUNCTION()
	void OnHealthChangedHandler(float OldHealth, float NewHealth);
	UFUNCTION()
	void OnDeathHandler();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UF1HealthComponent> HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UTextRenderComponent> TextRenderComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "F1|Input")
	TObjectPtr<UF1InputSettings> InputSettings;

	float DefaultMaxSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "F1|Damage", meta = (UIMin = 0, ClampMin = 0))
	float DeathLifeSpan = 3.5f; 
	UPROPERTY(EditDefaultsOnly, Category = "F1|Animations")
	TObjectPtr<UAnimMontage> DeathAnimMontage;
};
