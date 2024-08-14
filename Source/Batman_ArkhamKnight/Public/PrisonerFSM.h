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
	Run,
	BackMove,
	RightAttack,
	LeftAttack,
	Damage,
	Faint,
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

	void SetState(EPrisonerState NextState);

	// 대기상태
	void IdleState(float& DeltaSeconds);
	// 이동상태
	void MoveState(float& DeltaSeconds);
	// 달리기상태
	void RunState(float& DeltaSeconds);
	// 뒷걸음상태
	void BackMoveState(float& DeltaSeconds);
	// 공격상태
	void RightAttackState(float& DeltaSeconds);
	// 공격상태
	void LeftAttackState(float& DeltaSeconds);
	// 피격상태
	void DamageState(float& DeltaSeconds);
	// 기절상태
	void FaintState(float& DeltaSeconds);
	// 죽음상태
	void DieState(float& DeltaSeconds);

	// 대기시간
	UPROPERTY(EditAnywhere,Category=PFSM)
	float idleDelayTime = 3;

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
	float moveDelayTime = 4;

	UPROPERTY(EditAnywhere,Category= PFSM)
	float backmoveDelayTime = 5;

	// attack
	UPROPERTY(EditAnywhere,Category= PFSM)
	float attackDelayTime = 1;

	UPROPERTY(EditAnywhere,Category= PFSM)
	float attackDistance = 100.0f;

	UFUNCTION()
	void OnPlayerHit();

	// damage
	void OnMyTakeDamage(int32 damage);


	// faint
	UPROPERTY(EditAnywhere,Category= PFSM)
	float FaintDelayTime = 8.0f;

	// HP
	
	UPROPERTY(EditAnywhere,Category= PFSM)
	float MaxHp = 4;
	float HP;

	// Die
	UPROPERTY(EditAnywhere,Category= PFSM)
	float DieDelayTime = 2.0f;

	void SetCollision(bool bvalue);

	UFUNCTION(BlueprintCallable)
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// player에게 attack을 하고 있는 상태를 알리기 위한 함수
	bool IsCounter();

	class APlayerGameModeBase* MyGameModeBase;

	//sound

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PrisonerDamageSound1;
	
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PrisonerDamageSound2;
	
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PrisonerDamageSound3;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PrisonerScream;

		UPROPERTY(EditDefaultsOnly)
	class USoundBase* PrisonerAttack;
};
