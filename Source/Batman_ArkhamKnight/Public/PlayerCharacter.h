// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
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

UENUM()
enum class EEnemyDirection
{
	Front,
	Back,
	Left,
	Right
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

	// 카메라
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

	// 키 입력
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

	// 애니메이션
	UPROPERTY(EditDefaultsOnly)
	class UPlayerAnim* PlayerAnim;
	// 애니메이션 몽타주
	UPROPERTY(EditAnywhere)
	class UAnimMontage* FrontAttackMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* BackAttackMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* DamageMontage;

	// 죄수
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> PrisonerFactory;
	UPROPERTY()
	class APrisoner* TargetEnemy;

	// 방향
	FVector Direction;

	// 움직임 여부
	bool bMoveInputPressed;
	bool bMovingToTarget;
	bool bRotatingToTarget;

	// 공격 
	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 1000;
	int32 AttackMaxSpeed = 3000;
	int32 DefaultMaxSpeed;
	int32 ComboCount = 0;

	// 회피
	UPROPERTY(EditDefaultsOnly)
	float DodgeSpeed = 1000;
	UPROPERTY(EditDefaultsOnly)
	float DoublePressInterval = 0.5f;
    float LastDodgeInputPressTime = 0;
    bool bDodgeInputPressed = false;

	// 피격
	UPROPERTY(EditDefaultsOnly)
	int32 MaxHP = 10;
	int32 HP;

	UPROPERTY(EditDefaultsOnly)
	float DamageIdleTime = 1;
	bool bDamageState;

	// TimerHandler
	FTimerHandle DamageTimerHandler;

private:
	void OnActionMove(const FInputActionValue& Value);
	void OnActionMoveCompleted(const FInputActionValue& Value);
	void OnActionLook(const FInputActionValue& Value);
	void OnActionDodge(const FInputActionValue& Value);
	void OnActionAttack(const FInputActionValue& Value);
	void OnPlayAttackAnimation();

	void MoveToTarget(AActor* Target);
	void RotateToTarget(AActor* Target);
	
	bool IsLockedMove() const;

	EEnemyDirection GetTargetVerticalDirection(AActor* TargetActor);
	EEnemyDirection GetTargetHorizontalDirection(AActor* TargetActor);

public:
	void ResetCombo();
	void SetMeshCollisionEnabled(bool bValue);

	void OnDamageProcess(AActor* OtherActor, int32 Damage);

	UFUNCTION(BlueprintCallable)
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
