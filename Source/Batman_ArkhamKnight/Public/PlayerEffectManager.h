// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerEffectManager.generated.h"

UENUM()
enum class EPlayerEffectType
{
	DefaultAttack,
	SpecialAttack,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UPlayerEffectManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerEffectManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TArray<class UNiagaraSystem*> DefaultAttackEffect;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UNiagaraSystem*> SpecialAttackEffect;

public:
	void SpawnEffectAtLocation(EPlayerEffectType EffectType, FVector SpawnLocation, FRotator SpawnRotation);

private:
	void SpawnEffect(TArray<class UNiagaraSystem*> Effects, FVector SpawnLocation, FRotator SpawnRotation);

};
