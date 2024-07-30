// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleEnemy.h"

#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-30
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

	Move();
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