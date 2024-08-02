// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerAnim.h"
#include "Prisoner.h"

void UPrisonerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APrisoner* prisoner = Cast<APrisoner>(TryGetPawnOwner());

	if (nullptr == prisoner)
	{
		return;
	}
	FVector velocity = prisoner->GetVelocity();
	FVector forwardvector = prisoner->GetActorForwardVector();
	Speed = FVector::DotProduct(forwardvector,velocity);
}
