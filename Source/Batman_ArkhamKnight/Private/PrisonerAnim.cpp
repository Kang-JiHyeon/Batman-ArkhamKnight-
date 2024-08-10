// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerAnim.h"
#include "Prisoner.h"
#include "PlayerCharacter.h"

void UPrisonerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	prisoner = Cast<APrisoner>(TryGetPawnOwner());

	if (nullptr == prisoner)
	{
		return;
	}
	FVector velocity = prisoner->GetVelocity();
	FVector forwardvector = prisoner->GetActorForwardVector();
	Speed = FVector::DotProduct(forwardvector,velocity);

	player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController());
}

void UPrisonerAnim::AnimNotify_AttackStart()
{
	// 이때 player에게 damage를 입히고 싶다.
	prisoner->fsm->OnPlayerHit();
}

