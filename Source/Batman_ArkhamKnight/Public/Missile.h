// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-28
 */

UCLASS()
class BATMAN_ARKHAMKNIGHT_API AMissile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileMesh", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* MissileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileMovement", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	AMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
