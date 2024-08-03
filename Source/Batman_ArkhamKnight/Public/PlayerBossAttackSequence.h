// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerBossAttackSequence.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UPlayerBossAttackSequence : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBossAttackSequence();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    //// 레벨 시퀀스 애셋
    //UPROPERTY(EditAnywhere, Category = "Camera")
    //class ULevelSequence* CameraAttackSequence;

    //// 레벨 시퀀스 플레이어
    //UPROPERTY()
    //class ULevelSequencePlayer* SequencePlayer;

    //// 레벨 시퀀스 액터
    //UPROPERTY()
    //class ALevelSequenceActor* SequenceActor;
};
