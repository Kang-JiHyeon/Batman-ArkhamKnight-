// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    auto ownerPawn = TryGetPawnOwner();
    auto player = Cast<APlayerCharacter>(ownerPawn);

    if (player != nullptr)
    {
        FVector velocity = player->GetVelocity();
        FVector forwordVector = player->GetActorForwardVector();
        Speed = FVector::DotProduct(forwordVector, velocity);

        auto movement = player->GetCharacterMovement();
        bDodge = movement->IsFalling();

        player->OnChangedDodgeSpeed(false);
    }
}

void UPlayerAnim::SetDodge(bool bValue)
{
    bDodge = bValue;
}
