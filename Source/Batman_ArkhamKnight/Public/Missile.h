// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-30
 */

class USkeletalMeshComponent;

struct FTimerHandle;

UCLASS()
class BATMAN_ARKHAMKNIGHT_API AMissile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MissileMesh;

	FTimerHandle MissileTimerHandle;

	FVector TargetLocation;
	FVector Direction = GetActorForwardVector();
	float MissileSpeed = 2000.f;
	
public:	
	// Sets default values for this actor's properties
	AMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTargetLocation(FVector Location) { TargetLocation = Location; }

	void Translate(float Time, const FVector& direction, float Speed);
	void TurnToTarget();
};