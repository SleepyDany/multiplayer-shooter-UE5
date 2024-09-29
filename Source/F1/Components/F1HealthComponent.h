// Project F1. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "F1HealthComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(F1LogHealthComponent, Log, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class F1_API UF1HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UF1HealthComponent();

	UFUNCTION(BlueprintCallable)
	void StartHeal();
	
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, OldHealth, float, NewHealth);
	FOnHealthChanged OnHealthChanged;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
		AActor* DamageCauser);
	UFUNCTION()
	virtual void OnHealHandler();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Health", meta = (UIMin = 0, ClampMin = 0))
	float DefaultHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Health|Heal")
	bool bAutoHeal = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Health|Heal", meta = (UIMin = 0, ClampMin = 0))
	float StartHealDelay = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Health|Heal", meta = (UIMin = 0, ClampMin = 0))
	float HealValue = 2.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "F1|Health|Heal", meta = (UIMin = 0, ClampMin = 0))
	float BetweenHealDelay = 1.0f;

private:
	void SetHealth(float NewHealth);
	
	float Health = 0.0f;

	FTimerHandle HealTimer;
};
