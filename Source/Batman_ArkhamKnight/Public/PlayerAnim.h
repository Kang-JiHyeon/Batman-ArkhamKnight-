// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

UENUM(BlueprintType)
enum class EComboAttackState : uint8
{
	Punching_Right,
	Punching_Left,
	Hook_Right
};

UCLASS()
class BATMAN_ARKHAMKNIGHT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;


public:

	UPROPERTY()
	class APlayerCharacter* Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDodge = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAttack = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EComboAttackState ComboAttackState;


	bool bIsAttack = false;


public:
	bool bIgnoreAttack = false;
	
	
	
	void SetDodge(bool bValue);
	void SetAttack(bool bValue);

	bool GetIsAttack();
	bool GetEnableCombo();
	void SetIsAttack(bool bValue);

	UFUNCTION(BlueprintCallable)
	void OnEnableCombo();
	
	UFUNCTION(BlueprintCallable)
	void OnDisableCombo();
	UFUNCTION(BlueprintCallable)
	void OnResetCombo();

	UFUNCTION(BlueprintCallable)
	void SetIgnoreAttack(bool bValue);




};
