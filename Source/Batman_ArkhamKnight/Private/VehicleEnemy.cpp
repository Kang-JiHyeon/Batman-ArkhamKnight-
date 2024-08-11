// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleEnemy.h"

#include "BaseWheeledVehiclePawn.h"
#include "Missile.h"
#include "PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/RadialForceComponent.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-08-08
 */

// Sets default values
AVehicleEnemy::AVehicleEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetRelativeLocation(FVector(10.f, 0.f, 60.f));
	Collision->SetBoxExtent(FVector(230.f,110.f,54.f));

	VehicleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMesh"));
	VehicleMesh->SetupAttachment(Collision);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(VehicleMesh);

	MissileSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MissileSpawnLocation"));
	MissileSpawnLocation->SetupAttachment(VehicleMesh);
	MissileSpawnLocation->SetRelativeLocation(FVector(-50.f, 0.f, 140.f));
	MissileSpawnLocation->SetRelativeRotation(FRotator(10.f, 0.f, 0.f));

	OnSplineTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	MoveOnSpline.BindUFunction(this, FName("Move"));
	
}

// Called when the game starts or when spawned
void AVehicleEnemy::BeginPlay()
{
	Super::BeginPlay();

	TrackSpline = TrackActor -> GetComponentByClass<USplineComponent>();
	OnSplineTimeline -> AddInterpFloat(TrackCurveFloat, MoveOnSpline);
	OnSplineTimeline -> SetLooping(false);

	//PossessCharacter = PossessCharacterClass -> GetDefaultObject<APlayerCharacter>();

	Move();
	Health = MaxHealth;

	GetWorld() -> GetTimerManager().SetTimer(MissileTimerHandle,
		this,
		&AVehicleEnemy::FireMissile,
		5.f,
		true,
		4.f);
}

// Called every frame
void AVehicleEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVehicleEnemy::Move()
{
	OnSplineTimeline -> SetPlayRate(1 / TimeToCompleteTrack);
	OnSplineTimeline -> Play();

	float distance = UKismetMathLibrary::Lerp(0, TrackSpline -> GetSplineLength(), TrackCurveFloat -> GetFloatValue(OnSplineTimeline -> GetPlaybackPosition()));
	
	FVector newLocation = TrackSpline -> GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	FRotator newRotation = TrackSpline -> GetRotationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	
	SetActorTransform(FTransform(newRotation, newLocation, FVector(1.f)));
}

void AVehicleEnemy::FireMissile()
{
	if(bIsAttackAble == false)
	{
		return;
	}
	AMissile* Missile = GetWorld() -> SpawnActor<AMissile>(MissileClass, MissileSpawnLocation -> GetComponentTransform());
	Missile -> SetTarget(GetWorld() -> GetFirstPlayerController() -> GetPawn());
}

void AVehicleEnemy::OnDamage(int Amount)
{
	Health -= Amount;
	if(Health <= 0)
	{
		VehicleMesh -> SetSimulatePhysics(true);
		VehicleMesh -> SetCollisionProfileName("Ragdoll");
		RadialForce -> FireImpulse();
		bIsAttackAble = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), .2f);
		GetWorld() -> GetTimerManager().SetTimer(TimeSleepHandle, FTimerDelegate::CreateLambda(
			[this]() -> void
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
			}),
			.1f,false, 1.f);
		/*GetWorld() -> GetTimerManager().ClearTimer(TimeSleepHandle);
		GetWorld() -> GetTimerManager().SetTimer(TimeSleepHandle, this, &AVehicleEnemy::PossessBatman, .1f, false, 1.f);*/
	}
}


