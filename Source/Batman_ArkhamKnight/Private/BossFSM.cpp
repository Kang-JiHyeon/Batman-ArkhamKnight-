// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
//#include "EnemyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Boss.h"
#include "BossAnim.h"
#include "PlayerCharacter.h"

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

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	Ptarget = Cast<APlayerCharacter>(actor);
	me = Cast<ABoss>(GetOwner());

	// animation
	anim = Cast<UBossAnim>(me->GetMesh()->GetAnimInstance());
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
	case EBossState::RightAttack:
		RightAttackState();
		break;
	case EBossState::LeftAttack:
		LeftAttackState();
		break;
	case EBossState::DoubleRightAttack:
		DoubleRightAttackState();
		break;
	case EBossState::DoubleLeftAttack:
		DoubleLeftAttackState();
		break;
	case EBossState::Damage:
		DamageState();
		break;
	case EBossState::Die:
		DieState();
		break;
	//case EBossState::SavePrisoner:
	//	SavePrisonerState();
	//	break;
	case EBossState::FastMove:
		FastMoveState();
		break;
	}
	
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
}

void UBossFSM::IdleState()
{
	
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{

		mState = EBossState::Move;
		currentTime = 0;
		anim->animState = mState;
	}
}

void UBossFSM::MoveState() // boss move to player or idle
{

	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > moveDelayTime)
	{
		int32 statevalue = FMath::RandRange(0, 9);
		if (statevalue < 8)
		{
			direction = Ptarget->GetActorLocation() - me->GetActorLocation();
			mState = EBossState::FastMove;
		}
		else if (statevalue == 8)
		{
			mState = EBossState::Move;
		}
		else {
			mState = EBossState::Idle;

		}
		currentTime = 0;
		anim->animState = mState;
	}
		//타깃의 목적지
	FVector destination = Ptarget->GetActorLocation();
		//방향
	FVector dir = destination - me->GetActorLocation();
		//방향으로 이동
	me->AddMovementInput(dir.GetSafeNormal(), 0.2f);

	if (dir.Size() < attackRange)
	{
		int32 attackstatevalue = FMath::RandRange(0, 5);
		if (attackstatevalue == 0)
		{
			if (FMath::RandBool()) {
				mState = EBossState::DoubleRightAttack;
				anim->animState = mState;
			}
			else {
				mState = EBossState::DoubleLeftAttack;
				anim->animState = mState;
			}
		}
		else if (attackstatevalue == 1 || attackstatevalue == 2)
		{
			anim->bAttackPlay = true;
			mState = EBossState::LeftAttack;
			anim->animState = mState;
			currentTime = attackDelayTime;
		}
		else
		{
			anim->bAttackPlay = true;
			mState = EBossState::RightAttack;
			anim->animState = mState;
			currentTime = attackDelayTime;
		}
	}


	// prisoner가 player보다 가깝고 기절상태일 때 줍는 상태도 필요

}

void UBossFSM::RightAttackState() // smash
{
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	currentTime += GetWorld()->DeltaTimeSeconds;
	anim->bAttackPlay = true;

	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
	}
	if (distance > 2 * attackRange)
	{
		mState = EBossState::Move;
		anim->animState = mState;
	}
}


void UBossFSM::LeftAttackState() // smash
{
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	currentTime += GetWorld()->DeltaTimeSeconds;
	anim->bAttackPlay = true;
	if (currentTime > attackDelayTime)
	{

		currentTime = 0;
	}
	if (distance > 2 *attackRange)
	{
		mState = EBossState::Move;
		anim->animState = mState;

	}
}

void UBossFSM::DoubleRightAttackState() // double smash
{
	UE_LOG(LogTemp, Warning, TEXT("DoubleRightAttack"));
	currentTime += GetWorld()->DeltaTimeSeconds;


	// double attack 2 type
	if (currentTime > attackDelayTime)
	{
		mState = EBossState::Move;
		anim->animState = mState;
		currentTime = 0;
	}
}

void UBossFSM::DoubleLeftAttackState() // double smash
{
	UE_LOG(LogTemp, Warning, TEXT("DoubleRightAttack"));
	currentTime += GetWorld()->DeltaTimeSeconds;


	// double attack 2 type
	if (currentTime > attackDelayTime)
	{
		mState = EBossState::Move;
		anim->animState = mState;
		currentTime = 0;
	}
}

void UBossFSM::DamageState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > damageDelayTime)
	{
		mState = EBossState::Idle;
		anim->animState = mState;
		currentTime = 0;
	}
}

void UBossFSM::DieState()
{
	me->Destroy();
}

//void UBossFSM::SavePrisoner() // boss hold prisoner
//{
//}



void UBossFSM::FastMoveState()
{
// player first position remember and go to there
// if player is there in boss root -> damage

	me->AddMovementInput(direction.GetSafeNormal(), 1.0f);
	
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > fastDelayTime)
	{
		mState = EBossState::Move;
		currentTime = 0;
		anim->animState = mState;
	}
}

void UBossFSM::OnDamageProcess()
{
	// according to attack, hp change

	BossHp--;

	if (BossHp > 0)
	{
		mState = EBossState::Damage;
	}
	else
	{
		mState = EBossState::Die;
	}
}

