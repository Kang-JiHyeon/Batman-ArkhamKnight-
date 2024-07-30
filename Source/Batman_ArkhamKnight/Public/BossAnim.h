// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossFSM.h"
#include "BossAnim.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=BossFSM)
	EBossState BanimState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= BossFSM)
	bool bAttackPlay = false;

	// attack animation end
	UFUNCTION(BlueprintCallable,Category=FSMEvent)
	void OnEndAttackAnim();

};
