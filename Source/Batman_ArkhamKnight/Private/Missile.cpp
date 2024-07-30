// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"


/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-30
 */

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissileMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MissileMesh"));
	SetRootComponent(MissileMesh);
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorRelativeRotation(Direction.Rotation());

	GetWorld()->GetTimerManager().SetTimer(MissileTimerHandle,
		this,
		&AMissile::TurnToTarget,
		.1f,
		false,
		1.f);
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Translate(DeltaTime, Direction, MissileSpeed);
}

void AMissile::Translate(float Time, const FVector& direction, float Speed)
{
	FVector NewLocation = GetActorLocation() + direction * Speed * Time;
	SetActorLocation(NewLocation);
}

void AMissile::TurnToTarget()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, GetActorLocation());
	SetActorRotation(TargetRotation);
	Direction = TargetLocation - GetActorLocation();
	Direction.Normalize();
	MissileSpeed = 30000.f;
}