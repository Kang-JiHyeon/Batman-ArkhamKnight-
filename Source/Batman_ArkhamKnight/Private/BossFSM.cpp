// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
#include "EnemyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Boss.h"

// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyPlayer::StaticClass());
	Ptarget = Cast<AEnemyPlayer>(actor);
	me = Cast<ABoss>(GetOwner());
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EBossState::Idle:
		IdleState();
		break;
	case EBossState::Move:
		MoveState();
		break;
	case EBossState::Attack:
		AttackState();
		break;
	case EBossState::DoubleAttack:
		DoubleAttackState();
		break;
	//case EBossState::Damage:
	//	DamageState();
	//	break;
	//case EBossState::Die:
	//	DieState();
	//	break;
	//case EBossState::SavePrisoner:
	//	SavePrisonerState();
	//	break;
	//case EBossState::FastMove:
	//	FastMoveState();
	//	break;
	}
	
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
}

void UBossFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{

		// 랜덤으로 일정시간이 지나면 이동하거나 죄수를 세우고 싶다.
		int32 statevalue = FMath::RandRange(0, 1);
		if (statevalue == 0)
		{
			mState = EBossState::Move;
		}
		else
		{
			/*mState = EBossState::SaveEnemy;*/
		}

		currentTime = 0;
	}
}

void UBossFSM::MoveState() // 보스가 플레이어로 이동하는 상태
{
	// 타깃의 목적지
	FVector destination = Ptarget->GetActorLocation();
	// 방향
	FVector dir = destination - me->GetActorLocation();
	// 방향으로 이동
	me->AddMovementInput(dir.GetSafeNormal(),0.2f);

	if (dir.Size() < attackRange)
	{
		mState = EBossState::Attack;
	}
	else
	{
		// 플레이어가 멀리 있으면 기어서 돌진
	}
	// prisoner가 player보다 가깝고 기절상태일 때 줍는 상태도 필요

}

void UBossFSM::AttackState() // 단타
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("attack"));
		currentTime = 0;
	}

	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	if (distance > attackRange)
	{
		mState = EBossState::Move;
	}

	if (distance < attackRange / 2)
	{
		mState = EBossState::DoubleAttack;
	}
}

void UBossFSM::DoubleAttackState() // 연타
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("doubleattack"));
		currentTime = 0;
	}
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	if (distance > attackRange/2)
	{
		mState = EBossState::Move;
	}
}

//void UBossFSM::DamageState()
//{
//}
//
//void UBossFSM::DieState()
//{
//}
//
//void UBossFSM::SavePrisoner() // 보스가 죄수(잡몹)에게로 이동하여 일으켜 세우는 상태
//{
//}
//
//void UBossFSM::FastMoveState()
//{
//}

