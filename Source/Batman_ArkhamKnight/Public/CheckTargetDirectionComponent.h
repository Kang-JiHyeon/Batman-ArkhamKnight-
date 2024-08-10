// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CheckTargetDirectionComponent.generated.h"

UENUM()
enum class EDirectionType
{
	Front,
	Back,
	Left,
	Right
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UCheckTargetDirectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCheckTargetDirectionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	EDirectionType GetTargetVerticalDirection(AActor* TargetActor);
	EDirectionType GetTargetHorizontalDirection(AActor* TargetActor);
};
