// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

UCLASS()
class BATMAN_ARKHAMKNIGHT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:

	UPROPERTY()
	class APlayerCharacter* Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDodge = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bRun = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDie = false;


public:
	bool bIgnoreInputAttack = false;
	
	void SetRun(bool bValue);
	void SetDodge(bool bValue);

	UFUNCTION(BlueprintCallable)
	void OnResetCombo();

	UFUNCTION(BlueprintCallable)
	void SetIgnoreAttack(bool bValue);

	UFUNCTION(BlueprintCallable)
	void OnEndDamageAnimation();
	UFUNCTION(BlueprintCallable)
	void OnStartSlowMotion();
	UFUNCTION(BlueprintCallable)
	void OnEndSlowMotion();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_OnHit();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_OnHitBoss();
};
