// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraShake.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShakeBase.h"

// Sets default values for this component's properties
UPlayerCameraShake::UPlayerCameraShake()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCameraShake::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerCameraShake::PlayCameraShake(ECameraShakeType State)
{
	TSubclassOf<UCameraShakeBase> ShakeClass;

	switch (State)
	{
	case ECameraShakeType::Attack:
		ShakeClass = AttackCameraShake;
		break;
	case ECameraShakeType::Damage:
		ShakeClass = DamageCameraShake;
		break;
	default:
		break;
	}

	if (ShakeClass != nullptr)
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ShakeClass);
}

