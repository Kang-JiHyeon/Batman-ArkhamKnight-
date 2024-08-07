// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "VehicleEnemy.generated.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-30
 */

class UArrowComponent;
class USplineComponent;
class UBoxComponent;
class USkeletalMeshComponent;
class UCurveFloat;

struct FTimerHandle;

UCLASS()
class BATMAN_ARKHAMKNIGHT_API AVehicleEnemy : public AActor
{
	GENERATED_BODY()

	//* Components */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Collision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VehicleMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* VehicleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpawnLocation", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* MissileSpawnLocation;

	//* Missile Class */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MissileClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AMissile> MissileClass;

	FTimerHandle MissileTimerHandle;

	//* Track and Spline */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	AActor* TrackActor;

	UPROPERTY()
	USplineComponent* TrackSpline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* OnSplineTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* TrackCurveFloat;
	
	FOnTimelineFloat MoveOnSpline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float TimeToCompleteTrack = 30.f;

	// * Stat */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	int Health;
	
public:	
	// Sets default values for this actor's properties
	AVehicleEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Move();

	void FireMissile();
	void OnDamage(int Amount);

	FORCEINLINE void SetHealth(int NewHealth) { Health = NewHealth; }
	FORCEINLINE int GetHealth() const { return Health; }
};
