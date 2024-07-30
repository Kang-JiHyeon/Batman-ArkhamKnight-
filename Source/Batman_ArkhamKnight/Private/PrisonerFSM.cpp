// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerFSM.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Prisoner.h"
#include "PrisonerAnim.h"

// Sets default values for this component's properties
UPrisonerFSM::UPrisonerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPrisonerFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	Ptarget = Cast<APlayerCharacter>(actor);
	me = Cast<APrisoner>(GetOwner());

	// animation
	anim = Cast<UPrisonerAnim>(me->GetMesh()->GetAnimInstance());
	
}


// Called every frame
void UPrisonerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EPrisonerState::Idle:
		IdleState(DeltaTime);
		break;
	case EPrisonerState::Move:
		MoveState(DeltaTime);
		break;
	case EPrisonerState::RightAttack:
		RightAttackState(DeltaTime);
		break;
	case EPrisonerState::LeftAttack:
		LeftAttackState(DeltaTime);
		break;
	case EPrisonerState::Damage:
		DamageState(DeltaTime);
		break;
	case EPrisonerState::Die:
		DieState(DeltaTime);
		break;
	}
}

void UPrisonerFSM::IdleState(float& DeltaSeconds)
{

	currentTime += DeltaSeconds;
	if (currentTime > idleDelayTime)
	{

		mState = EPrisonerState::Move;
		currentTime = 0;
		anim->PanimState = mState;
	}
}

void UPrisonerFSM::MoveState(float& DeltaSeconds)
{
	// 타깃의 목적지
	FVector destination = Ptarget->GetActorLocation();
	// 
	FVector dir = destination - me->GetActorLocation();
	// 모두 다 플레이어를 향하지 않고 몇몇은 뒷걸음질 치고 싶다.
	//if (FMath::RandBool())
	//{
		//방향으로 이동
		me->AddMovementInput(dir.GetSafeNormal(), 0.05f);
	//}
	//else
	//{
	//	// 뒷걸음질 치고 싶다

	//	me->AddMovementInput(dir.GetSafeNormal(), 1.0f);

	//}
	


	// 공격범위내에 플레이어가 들어오면 공격하고 싶다. 
	float distance = dir.Size();
	if (distance < attackDistance)
	{
		// 오른쪽 공격과 왼쪽 공격을 랜덤하게 나오게 하고 싶다.
		if (FMath::RandBool())
		{
			mState = EPrisonerState::LeftAttack;
		}
		else
		{
			mState = EPrisonerState::RightAttack;
		}
	}

}

void UPrisonerFSM::RightAttackState(float& DeltaSeconds)
{
	// 공격범위내에 플레이어가 들어오고 나서 1초 후에 공격하고 싶다.
	// 오른쪽 펀치를 하고 싶다.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		// 펀치를 하고 난 후 다시 이동으로 전이하고 싶다.
		float dist = me->GetDistanceTo(Ptarget);
		if (dist < attackDistance)
		{
			anim->PanimState = mState;
		}
		else
		{
			mState = EPrisonerState::Move;
			anim->PanimState = mState;
		}
	}
	

}

void UPrisonerFSM::LeftAttackState(float& DeltaSeconds)
{
	// 공격범위내에 플레이어가 들어오고 나서 1초 후에 공격하고 싶다.
	// 왼쪽 펀치를 하고 싶다.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		// 펀치를 하고 난 후 다시 이동으로 전이하고 싶다.
		float dist = me->GetDistanceTo(Ptarget);
		if (dist < attackDistance)
		{
			anim->PanimState = mState;
		}
		else
		{
			mState = EPrisonerState::Move;
			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::DamageState(float& DeltaSeconds)
{

}

void UPrisonerFSM::DieState(float& DeltaSeconds)
{
	me->Destroy();
}

void UPrisonerFSM::OnMyTakeDamage(int32 damage)
{
	HP -= damage;
	if (HP > 0)
	{
		mState = EPrisonerState::Damage;
	}
	else
	{
		mState = EPrisonerState::Die;
	}
}
