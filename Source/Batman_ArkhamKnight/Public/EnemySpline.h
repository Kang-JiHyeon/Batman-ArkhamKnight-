// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpline.generated.h"

class USplineComponent;

UCLASS()
class BATMAN_ARKHAMKNIGHT_API AEnemySpline : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	USplineComponent* EnemySpline;
	
public:	
	// Sets default values for this actor's properties
	AEnemySpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
