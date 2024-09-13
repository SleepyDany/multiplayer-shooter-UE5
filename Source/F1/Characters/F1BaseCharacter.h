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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "F1|Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "F1|Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "F1|Input")
	TObjectPtr<UF1InputSettings> InputSettings;
};
