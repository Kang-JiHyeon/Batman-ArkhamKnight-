// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckTargetDirection.h"

// Sets default values for this component's properties
UCheckTargetDirection::UCheckTargetDirection()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCheckTargetDirection::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

EDirectionType UCheckTargetDirection::GetTargetVerticalDirection(AActor* Target)
{
	// 적이 앞에 있는지, 뒤에 있는지 판별
	FVector targetVector = Target->GetActorLocation() - GetOwner()->GetActorLocation();
	FVector forwardVector = GetOwner()->GetActorLocation();

	float forwardDotProduct = FVector::DotProduct(targetVector, forwardVector);

	if (forwardDotProduct > 0)
		return EDirectionType::Front;
	else
		return EDirectionType::Back;
}

EDirectionType UCheckTargetDirection::GetTargetHorizontalDirection(AActor* Target)
{
	// 적이 왼쪽에 있는지, 오른쪽에 있는지 판별
	FVector targetVector = Target->GetActorLocation() - GetOwner()->GetActorLocation();
	FVector rightVector = GetOwner()->GetActorLocation();

	float rightDotProject = FVector::DotProduct(targetVector, rightVector);

	if (rightDotProject > 0)
		return EDirectionType::Right;
	else
		return EDirectionType::Left;
}
