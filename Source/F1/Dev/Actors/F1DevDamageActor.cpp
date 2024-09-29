// Project F1. All Rights Reserved.
#include "F1DevDamageActor.h"

#include "Kismet/GameplayStatics.h"

AF1DevDamageActor::AF1DevDamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

void AF1DevDamageActor::BeginPlay()
{
	Super::BeginPlay();

	const auto World = GetWorld();
	check(World);

	auto& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(DamageTimer, this, &ThisClass::ApplyDamage, DamageCooldown, true);

	if (bDebug)
	{
		CheckDebug();
	}
}

void AF1DevDamageActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (const auto World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DamageTimer);
		World->GetTimerManager().ClearTimer(DebugTimer);
	}
}

void AF1DevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AF1DevDamageActor::TurnDebug(bool bEnabled)
{
	bDebug = bEnabled;
	CheckDebug();
}

void AF1DevDamageActor::CheckDebug()
{
	const auto World = GetWorld();
	check(World);

	auto& TimerManager = World->GetTimerManager();
	if (!bDebug)
	{
		TimerManager.ClearTimer(DebugTimer);
		return;
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, DebugSegments, DebugColor, false, DebugDrawCooldown, 0, DebugThickness);
	if (!TimerManager.TimerExists(DebugTimer))
	{
		TimerManager.SetTimer(DebugTimer, this, &ThisClass::CheckDebug, DebugDrawCooldown, true);
	}
}

void AF1DevDamageActor::ApplyDamage()
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, DamageType, {}, this, nullptr, bFullDamage);
}
