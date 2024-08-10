// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"
#include "Boss.h"

void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	boss = Cast<ABoss>(TryGetPawnOwner());

	if (nullptr == boss)
	{
		return;
	}
}


void UBossAnim::OnEndAttackAnim()
{
	bAttackPlay = false;
}

void UBossAnim::AnimNotify_AttackStart()
{
	boss->fsm->OnPlayerHit();
}

void UBossAnim::AnimNotify_ComboStart()
{
	boss->Damaged = true;
}


