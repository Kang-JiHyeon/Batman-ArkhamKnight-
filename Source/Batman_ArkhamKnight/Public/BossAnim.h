// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossFSM.h"
#include "PlayerAttackPointComponent.h"
#include "BossAnim.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=BossFSM)
	EBossState BanimState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= BossFSM)
	bool bAttackPlay = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = BossFSM)
	EAttackType attacktype;

	// attack animation end
	UFUNCTION(BlueprintCallable,Category=FSMEvent)
	void OnEndAttackAnim();

	UFUNCTION()
	void AnimNotify_AttackStart();

	UPROPERTY()
	class ABoss* boss;

	UFUNCTION()
	void AnimNotify_ComboStart();



};
