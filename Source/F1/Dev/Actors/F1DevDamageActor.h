// Project F1. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "F1DevDamageActor.generated.h"

UCLASS()
class F1_API AF1DevDamageActor : public AActor
{
	GENERATED_BODY()

public:
	AF1DevDamageActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Debug)
	void TurnDebug(bool bEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Damage", meta = (UIMin = 0, ClampMin = 0))
	float Damage = 2.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Damage", meta = (UIMin = 0, ClampMin = 0))
	float DamageRadius = 300.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Damage", meta = (UIMin = 0, ClampMin = 0))
	float DamageCooldown = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Damage", meta = (UIMin = 0, ClampMin = 0))
	bool bFullDamage = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Damage")
	TSubclassOf<UDamageType> DamageType = nullptr;

private:
	UFUNCTION()
	void ApplyDamage();
	UFUNCTION()
	void CheckDebug();

	FTimerHandle DamageTimer;
	FTimerHandle DebugTimer;

	UPROPERTY()
	bool bDebug = true;
	UPROPERTY(EditDefaultsOnly, Category = "F1|Debug")
	float DebugDrawCooldown = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "F1|Debug")
	FColor DebugColor = FColor::Red;
	UPROPERTY(EditDefaultsOnly, Category = "F1|Debug")
	float DebugThickness = 2.0f;
	UPROPERTY(EditDefaultsOnly, Category = "F1|Debug")
	int32 DebugSegments = 16;
};
