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
	Attack,
	DoubleAttack,
	//Damage,
	//Die,
	//SaveEnemy,
	//FastMove,

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EBossState mState = EBossState::Idle;

	// 대기상태
	void IdleState();
	// 이동상태
	void MoveState();
	// 공격상태(단타)
	void AttackState();
	// 공격상태(연타)
	void DoubleAttackState();
	//// 피격상태
	//void DamageState();
	//// 죽음상태
	//void DieState();
	//// 죄수줍기상태
	//void SavePrisonerState();
	//// 구르기상태
	//void FastMoveState();

	// 대기시간
	UPROPERTY(EditAnywhere,Category=FSM)
	float idleDelayTime = 2;
	// 경과시간(추가되어 대기시간을 초과할 시간)
	float currentTime = 0;

	// 타깃 플레이어
	UPROPERTY(EditAnywhere,Category=FSM)
	class AEnemyPlayer* Ptarget;

	// 나의 위치 기억
	UPROPERTY(EditAnywhere)
	class ABoss* me;
	
	UPROPERTY(EditAnywhere)
	float attackRange = 250.f;

	UPROPERTY(EditAnywhere)
	float attackDelayTime = 2.0f;
		
};
