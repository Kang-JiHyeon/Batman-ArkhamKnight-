// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFSM.generated.h"

UENUM(BlueprintType)
enum class EBossState :uint8
{
	Idle,
	Move,
	RightAttack,
	LeftAttack,
	DoubleRightAttack,
	DoubleLeftAttack,
	Damage,
	Die,
	SavePrisoner,
	FastMove,

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BFSM)
	EBossState mState = EBossState::Idle;

	// 대기상태
	void IdleState();
	// 이동상태
	void MoveState();
	// 공격상태(단타)
	void RightAttackState();
	// 공격상태(단타)
	void LeftAttackState();
	// 공격상태(연타)
	void DoubleRightAttackState();
	// 공격상태(연타)
	void DoubleLeftAttackState();
	// 피격상태
	void DamageState();
	// 죽음상태
	void DieState();
	// 죄수줍기상태
	void SavePrisonerState();
	// 기어가기 상태
	void FastMoveState();

	// 대기시간
	UPROPERTY(EditAnywhere,Category= BFSM)
	float idleDelayTime = 2;
	// 경과시간(추가되어 대기시간을 초과할 시간)
	float currentTime = 0;

	// 타깃 플레이어
	UPROPERTY(EditAnywhere,Category= BFSM)
	class APlayerCharacter* Ptarget;

	// 타깃 위치
	UPROPERTY(EditAnywhere)
	FVector direction;

	// 나의 위치 기억
	UPROPERTY(EditAnywhere)
	class ABoss* me;
	
	UPROPERTY(EditAnywhere)
	float attackRange = 250.f;

	UPROPERTY(EditAnywhere)
	float attackDelayTime = 1.5f;

	// move or idle
	UPROPERTY(EditAnywhere,Category= BFSM)
	float moveDelayTime = 2;

	// fastmove
	UPROPERTY(EditAnywhere,Category= BFSM)
	float fastDelayTime =2;

	UPROPERTY(EditAnywhere,Category= BFSM)
	float fastRange = 100.0f;

	// animation
	UPROPERTY()
	class UBossAnim* anim;

	// damage
	void OnDamageProcess();

	UPROPERTY(EditAnywhere,Category= BFSM)
	float damageDelayTime = 2;

	// hp
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category= BFSM)
	int32 BossHp = 10;


		
};
