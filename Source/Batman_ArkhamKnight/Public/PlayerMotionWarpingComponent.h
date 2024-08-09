// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackPointComponent.h"
#include "PlayerMotionWarpingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UPlayerMotionWarpingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMotionWarpingComponent();

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

	TMap<EAttackType, class UPlayerAttackPointComponent*> AttackPoints;


	void OnInitialize(AActor* OtherActor);

	void AddAndUpdateMotionWarping(EAttackType AttackType);
	void RemoveMotionWarping();

};
