// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerAttackPointComponent.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class EPlayerState
{
	Idle, 
	Move,
	Attack,
	Dodge,
	Damage,
	Die
};



UCLASS()
class BATMAN_ARKHAMKNIGHT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ī�޶�
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

	// �Ž�-����
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* CapeMeshComp;

	// Ű �Է�
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMP_Player;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Dodge;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Attack;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_BossAttack;

	// �ִϸ��̼�
	UPROPERTY(EditDefaultsOnly)
	class UPlayerAnim* PlayerAnim;
	// �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditAnywhere)
	class UAnimMontage* FrontAttackMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* BackAttackMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* DamageMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* BossAttackMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere)
	TArray<class UAnimMontage*> BossAttackMotages;

	int32 bossAttackIndex;

	// ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class ABoss* TargetBoss;
	
	// �˼�
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> PrisonerFactory;

	UPROPERTY(VisibleDefaultsOnly)
	class APrisoner* TargetPrisoner;
	UPROPERTY()
	class APrisoner* OverlapPrisoner;

	// ����
	FVector Direction;

	// ������ ����
	bool bMoveInputPressed;
	bool bMovingToTarget;
	bool bRotatingToTarget;

	// �˼� ���� 
	UPROPERTY()
	class UCheckTargetDirectionComponent* CheckTargetDirComp;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 1000;
	int32 AttackMaxSpeed = 3000;
	int32 DefaultMaxSpeed;
	int32 AnimComboCount = 0;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxHitCombo = 12;
	int32 HitCombo = 0;

	// ���� ����
	UPROPERTY(EditDefaultsOnly)
	int32 MaxBossAttackCount = 8;
	int32 BossAttackCount = 0;
	EAttackType CurrAttackType;


	// ȸ��
	UPROPERTY(EditDefaultsOnly)
	float DodgeSpeed = 700;
	UPROPERTY(EditDefaultsOnly)
	float DoublePressInterval = 0.5f;
    float LastDodgeInputPressTime = 0;
    bool bDodgeInputPressed = false;

	// �ǰ�
	UPROPERTY(EditDefaultsOnly)
	int32 MaxHP = 20;
	int32 HP;

	UPROPERTY(EditDefaultsOnly)
	float DamageIdleTime = 1.5f;
	bool bDamageState;

	// TimerHandler
	FTimerHandle DamageTimerHandler;

	// GameMode
	class APlayerGameModeBase* MyGameModeBase;

	// Motion Warping
	UPROPERTY(EditDefaultsOnly)
	class UMotionWarpingComponent* MotionWarpingComp;
	UPROPERTY(EditDefaultsOnly)
	class UPlayerMotionWarpingComponent* PlayerMotionWarpingComp;


private:
	// Input
	void OnActionMove(const FInputActionValue& Value);
	void OnActionMoveCompleted(const FInputActionValue& Value);
	void OnActionLook(const FInputActionValue& Value);
	void OnActionDodge(const FInputActionValue& Value);
	void OnActionAttack(const FInputActionValue& Value);
	void OnActionBossAttack(const FInputActionValue& Value);

	// Move
	void MoveToTarget(AActor* Target);
	void RotateToTarget(AActor* Target);
	bool IsLockedMove() const;

	// Find Target
	APrisoner* FindTargetPrisoner();

	// Animation
	void PlayAttackAnimation();

	void SetHP(float Value);
	void SetHitCombo(float Value);
	void SetBossAttackCombo(float Value);
	void OnHitSucceeded(float Value);

public:
	bool bIsSlow;

	void OnTakeDamage(AActor* OtherActor, int32 Damage);
	void OnEndDamage();
	
	void OnHitPrisoner();
	void OnHitBoss();

	void OnPlayMotionWarping(EAttackType AttackType);

};
