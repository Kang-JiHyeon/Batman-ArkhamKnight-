// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-08-12
 */

class USkeletalMeshComponent;
class UCameraShakeBase;
class UCapsuleComponent;

struct FTimerHandle;

UCLASS()
class BATMAN_ARKHAMKNIGHT_API AMissile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* MissileCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MissileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> DamageCameraShake;

	//* SFX */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	USoundBase* ExplosionSound;

	UPROPERTY()
	AActor* TargetActor;

	FTimerHandle MissileTimerHandle;

	FVector TargetLocation;
	FVector Direction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpeed", meta = (AllowPrivateAccess = "true"))
	float MissileSpeed = 5000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpeed", meta = (AllowPrivateAccess = "true"))
	float AfterSpeed = 30000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpeed", meta = (AllowPrivateAccess = "true"))
	float TurnDelay = .5f;
	
public:	
	// Sets default values for this actor's properties
	AMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTarget(AActor* Target) { TargetActor = Target; }

	void Translate(float Time, const FVector& direction, float Speed);
	void TurnToTarget();
	void UpdateTargetLocation();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};