// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackPointComponent.h"
#include "PlayerMotionWarping.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UPlayerMotionWarping : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMotionWarping();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
	class APlayerCharacter* Me;

	UPROPERTY()
	class AActor* TargetActor;

	UPROPERTY(VisibleAnywhere)
	TMap<EAttackType, class UPlayerAttackPointComponent*> AttackPoints;


	void OnInitialize(AActor* OtherActor);

	void AddAndUpdateMotionWarping(EAttackType AttackType);

	void PlayMotionWarpingToTarget(AActor* TargetActor, float Offset);


	FName GetEnumValue(EAttackType AttackType);
};
