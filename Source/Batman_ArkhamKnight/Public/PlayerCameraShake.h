// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCameraShake.generated.h"

UENUM()
enum class ECameraShakeType
{
	Attack,
	Damage,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATMAN_ARKHAMKNIGHT_API UPlayerCameraShake : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerCameraShake();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> AttackCameraShake;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> DamageCameraShake;

public:
	void PlayCameraShake(ECameraShakeType State);
};
