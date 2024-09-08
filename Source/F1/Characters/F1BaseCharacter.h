// Copyright Project F1. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "F1BaseCharacter.generated.h"

class UCameraComponent;
DECLARE_LOG_CATEGORY_EXTERN(F1LogBaseCharacter, Log, All);

UCLASS()
class AF1BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AF1BaseCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UCameraComponent> CameraComponent;	
};
