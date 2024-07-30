// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PrisonerFSM.generated.h"

UENUM(BlueprintType)
enum class EPrisonerState :uint8
{
	Idle,
	Move,
	BackMove,
	RightAttack,
	LeftAttack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UPrisonerFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPrisonerFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PFSM)
	EPrisonerState mState = EPrisonerState::Idle;

	// 대기상태
	void IdleState(float& DeltaSeconds);
	// 이동상태
	void MoveState(float& DeltaSeconds);
	// 뒷걸음상태
	void BackMoveState(float& DeltaSeconds);
	// 공격상태
	void RightAttackState(float& DeltaSeconds);
	// 공격상태
	void LeftAttackState(float& DeltaSeconds);
	// 피격상태
	void DamageState(float& DeltaSeconds);
	// 죽음상태
	void DieState(float& DeltaSeconds);

	// 대기시간
	UPROPERTY(EditAnywhere,Category=PFSM)
	float idleDelayTime = 2;

	// 경과시간(추가되어 대기시간을 초과할 시간)
	float currentTime = 0;
	// 타깃 플레이어
	UPROPERTY(EditAnywhere,Category= PFSM)
	class APlayerCharacter* Ptarget;
	// 나의 위치 기억
	UPROPERTY(EditAnywhere)
	class APrisoner* me;
	// animation
	UPROPERTY()
	class UPrisonerAnim* anim;

	// move or idle
	UPROPERTY(EditAnywhere,Category= PFSM)
	float moveDelayTime = 2;

	UPROPERTY(EditAnywhere,Category= PFSM)
	float backmoveDelayTime = 4;

	// attack
	UPROPERTY(EditAnywhere,Category= PFSM)
	float attackDelayTime = 1;

	UPROPERTY(EditAnywhere,Category= PFSM)
	float attackDistance = 50.0f;

	// damage
	void OnMyTakeDamage(int32 damage);

	// HP
	
	UPROPERTY(EditAnywhere,Category= PFSM)
	int32 MaxHp = 2;
	int32 HP = MaxHp;

		
};
