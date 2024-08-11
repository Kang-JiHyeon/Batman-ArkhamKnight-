// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackPointComponent.h"
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
	Crawl,
	Yell,

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

	// ������
	void IdleState();
	// �̵�����
	void MoveState();
	// ���ݻ���(��Ÿ)
	void RightAttackState();
	// ���ݻ���(��Ÿ)
	void LeftAttackState();
	// ���ݻ���(��Ÿ)
	void DoubleRightAttackState();
	// ���ݻ���(��Ÿ)
	void DoubleLeftAttackState();
	// �ǰݻ���
	void DamageState();
	// ��������
	void DieState();
	// �Ҹ������� ����
	void YellState();
	// ���� ����
	void CrawlState();
	// �޺� �ǰݽ� ��� ����
	void SetupBossStateIdle();

	// ���ð�
	UPROPERTY(EditAnywhere,Category= BFSM)
	float idleDelayTime = 2;
	// ����ð�(�߰��Ǿ� ���ð��� �ʰ��� �ð�)
	float currentTime = 0;

	// Ÿ�� �÷��̾�
	UPROPERTY(EditAnywhere,Category= BFSM)
	class APlayerCharacter* Ptarget;

	// Ÿ�� ��ġ
	UPROPERTY(EditAnywhere)
	FVector direction;

	// ���� ��ġ ���        
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
	float fastRange =  50.0f;

	// animation
	UPROPERTY()
	class UBossAnim* anim;

	UPROPERTY(EditAnywhere,Category= BFSM)
	float damageDelayTime = 1;

	// hp
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category= BFSM)
	int32 BossHp = 10;
	int32 HP;

	UPROPERTY(EditDefaultsOnly)
	class UBossHP* BossHPWidget;

	// damage�� ������ ����
	UFUNCTION()
	void OnPlayerHit();

	UFUNCTION(BlueprintCallable)
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// fast move�߿� player�� sphere�� overlap�Ǹ� �Ͼ��
	UFUNCTION(BlueprintCallable)
	void OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// damage and die
	void OnMyTakeDamage(EAttackType attacktype, int32 damage);

	// crawl camera
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> CrawlCameraShake;
		

	void SetCollision(bool bvalue);

	class APlayerGameModeBase* MyGameModeBase;


	//sound

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BossRoarSound;

	UPROPERTY(EditAnywhere)
	class APrisoner* prisoner;
};
