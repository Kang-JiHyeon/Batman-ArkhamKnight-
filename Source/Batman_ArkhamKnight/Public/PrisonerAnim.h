// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PrisonerFSM.h"
#include "PrisonerAnim.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UPrisonerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PrisonerFSM)
	EPrisonerState PanimState;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=PrisonerFSM)
	bool attack;

	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=PrisonerFSM)
	float Speed;

	UPROPERTY()
	class APlayerCharacter* player;

	UPROPERTY()
	class APrisoner* prisoner;


	UFUNCTION()
	void AnimNotify_AttackStart();

	UPROPERTY()
	class UPrisonerFSM* fsm;
};
