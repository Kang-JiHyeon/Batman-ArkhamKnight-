// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEffectManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values for this component's properties
UPlayerEffectManager::UPlayerEffectManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerEffectManager::BeginPlay()
{
	Super::BeginPlay();

}


void UPlayerEffectManager::SpawnEffectAtLocation(EPlayerEffectType EffectType, FVector SpawnLocation, FRotator SpawnRotation)
{
	switch (EffectType)
	{
	case EPlayerEffectType::DefaultAttack:
		SpawnEffect(DefaultAttackEffect, SpawnLocation, SpawnRotation);
		break;
	case EPlayerEffectType::SpecialAttack:
		SpawnEffect(SpecialAttackEffect, SpawnLocation, SpawnRotation);
		break;
	default:
		break;
	}
}

void UPlayerEffectManager::SpawnEffect(TArray<class UNiagaraSystem*> Effects, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (Effects.Num() <= 0) return;

	for (auto effect : Effects)
	{
		if (effect == nullptr) continue;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), effect, SpawnLocation, SpawnRotation);
	}
}

