// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "VehicleEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"


/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-30
 */

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissileCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MissileCollision"));
	SetRootComponent(MissileCollision);

	MissileMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MissileMesh"));
	MissileMesh->SetupAttachment(RootComponent);

	MissileCollision->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
	Direction = GetActorForwardVector();
	SetActorRotation(Direction.ToOrientationRotator());

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
	MissileSpeed = 20000.f;
}

void AMissile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AVehicleEnemy* EnemyVehicle = Cast<AVehicleEnemy>(OtherActor))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Hit"));
		Destroy();
		EnemyVehicle -> SetHealth(EnemyVehicle -> GetHealth() - 1);
		if(EnemyVehicle -> GetHealth() <= 0)
		{
			EnemyVehicle -> Destroy();
		}
	}
}