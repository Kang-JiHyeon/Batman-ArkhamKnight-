// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

#include "BaseWheeledVehiclePawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "VehicleEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"


/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-08-04
 */

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissileCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MissileCollision"));
	SetRootComponent(MissileCollision);
	MissileCollision->SetCapsuleSize(10.f, 10.f);

	MissileMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MissileMesh"));
	MissileMesh->SetupAttachment(RootComponent);

	MissileVFX = CreateDefaultSubobject<UParticleSystem>(TEXT("MissileVFX"));
	

	MissileCollision->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();

	if(FireSound != nullptr)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}
	
	
	Direction = GetActorForwardVector();
	SetActorRotation(Direction.ToOrientationRotator());

	GetWorld()->GetTimerManager().SetTimer(MissileTimerHandle,
		this,
		&AMissile::TurnToTarget,
		.1f,
		false,
		TurnDelay);
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Translate(DeltaTime, Direction, MissileSpeed);

	if(TargetActor != nullptr)
	{
		UpdateTargetLocation();
	}
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
	MissileSpeed = AfterSpeed;
}

void AMissile::UpdateTargetLocation()
{
	TargetLocation = TargetActor -> GetActorLocation();
}


void AMissile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ExplosionSound != nullptr)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());	
	}

	if(MissileVFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MissileVFX, GetActorLocation(), FRotator::ZeroRotator, FVector(7.f), true, EPSCPoolMethod::AutoRelease);
	}
	
	if(AVehicleEnemy* EnemyVehicle = Cast<AVehicleEnemy>(OtherActor))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Enemy Hit"));
		Destroy();
		EnemyVehicle -> OnDamage(1);
	}

	else if(ABaseWheeledVehiclePawn* BatMobile = Cast<ABaseWheeledVehiclePawn>(OtherActor))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("BatMobile Hit"));
		UGameplayStatics::GetPlayerController(GetWorld(), 0) -> PlayerCameraManager -> PlayWorldCameraShake(GetWorld(), DamageCameraShake, GetActorLocation(), 10.f, 1000.f, 1.f, false);
		Destroy();
	}
}