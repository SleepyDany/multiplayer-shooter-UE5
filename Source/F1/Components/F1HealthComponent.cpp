// Project F1. All Rights Reserved.
#include "F1HealthComponent.h"

DEFINE_LOG_CATEGORY(F1LogHealthComponent);

UF1HealthComponent::UF1HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UF1HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(DefaultHealth);
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(0.f, Health);
	}

	if (const auto Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddUniqueDynamic(this, &ThisClass::OnTakeAnyDamageHandler);
	}
}

void UF1HealthComponent::SetHealth(float NewHealth)
{
	const float OldHealth = Health;
	Health = FMath::Clamp(NewHealth, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(OldHealth, Health);
}

float UF1HealthComponent::GetHealth() const
{
	return Health;
}

bool UF1HealthComponent::IsDead() const
{
	return FMath::IsNearlyZero(Health);
}

void UF1HealthComponent::OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(HealTimer);
	SetHealth(Health - Damage);

	if (IsDead())
	{
		OnDeath.Broadcast();
		return;
	}

	if (bAutoHeal)
	{
		StartHeal();
	}
}

void UF1HealthComponent::StartHeal()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimer, this, &ThisClass::OnHealHandler, BetweenHealDelay, true, StartHealDelay);
	}
}

void UF1HealthComponent::OnHealHandler()
{
	SetHealth(Health + HealValue);

	if (FMath::IsNearlyEqual(Health, DefaultHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimer);
	}
}
