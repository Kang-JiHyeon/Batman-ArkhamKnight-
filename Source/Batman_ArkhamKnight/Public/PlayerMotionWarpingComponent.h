// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMotionWarpingComponent.generated.h"

UENUM()
enum class EWarpingPoint : uint8
{
	Origin,
	Front,
	Right,
};

USTRUCT(Atomic)
struct FWarpingTransform
{
	GENERATED_USTRUCT_BODY()
public:
	FVector RelativeLocation;
	FRotator RelativeRotation;
};

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

	TMap<EWarpingPoint, FWarpingTransform> WarpingInfos;

	void OnInitialize();

	void AddMotionWarping(EWarpingPoint TargetPoint);
	void RemoveMotionWarping(EWarpingPoint TargetPoint);

};
