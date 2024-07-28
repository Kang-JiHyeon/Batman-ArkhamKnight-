// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

#include "GameFramework/ProjectileMovementComponent.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-28
 */

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissileMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MissileMesh"));
	SetRootComponent(MissileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement -> SetUpdatedComponent(RootComponent);

	ProjectileMovement -> InitialSpeed = 200.f;
	ProjectileMovement -> MaxSpeed = 10000.f;
	ProjectileMovement -> bRotationFollowsVelocity = true;

	ProjectileMovement -> bIsHomingProjectile = true;
	ProjectileMovement -> HomingAccelerationMagnitude = 10000.f;
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

