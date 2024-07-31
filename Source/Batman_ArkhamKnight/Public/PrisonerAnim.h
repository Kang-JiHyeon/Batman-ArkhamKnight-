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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PrisonerFSM)
	int32 HP;
};
