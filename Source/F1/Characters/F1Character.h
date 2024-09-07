// Copyright Project F1. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "F1Character.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(F1LogCharacter, Log, All);

UCLASS()
class AF1Character : public ACharacter
{
	GENERATED_BODY()

public:
	AF1Character();

protected:
	virtual void BeginPlay() override;
};
