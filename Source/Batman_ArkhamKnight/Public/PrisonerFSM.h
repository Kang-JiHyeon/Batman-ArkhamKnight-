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

	// ������
	void IdleState(float& DeltaSeconds);
	// �̵�����
	void MoveState(float& DeltaSeconds);
	// �޸������
	void RunState(float& DeltaSeconds);
	// �ް�������
	void BackMoveState(float& DeltaSeconds);
	// ���ݻ���
	void RightAttackState(float& DeltaSeconds);
	// ���ݻ���
	void LeftAttackState(float& DeltaSeconds);
	// �ǰݻ���
	void DamageState(float& DeltaSeconds);
	// ��������
	void FaintState(float& DeltaSeconds);
	// ��������
	void DieState(float& DeltaSeconds);

	// ���ð�
	UPROPERTY(EditAnywhere,Category=PFSM)
	float idleDelayTime = 3;

	// ����ð�(�߰��Ǿ� ���ð��� �ʰ��� �ð�)
	float currentTime = 0;
	// Ÿ�� �÷��̾�
	UPROPERTY(EditAnywhere,Category= PFSM)
	class APlayerCharacter* Ptarget;
	// ���� ��ġ ���
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
	UPROPERTY(EditAnywhere,Category= PFSM)
	float damageDelayTime = 0.5f;

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

	// player���� attack�� �ϰ� �ִ� ���¸� �˸��� ���� �Լ�
	bool IsAttack();

	class APlayerGameModeBase* MyGameModeBase;

};
