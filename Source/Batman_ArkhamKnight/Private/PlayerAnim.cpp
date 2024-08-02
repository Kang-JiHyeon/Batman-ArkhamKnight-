// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    auto ownerPawn = TryGetPawnOwner();
    Player = Cast<APlayerCharacter>(ownerPawn);

    if (Player != nullptr)
    {
        FVector velocity = Player->GetVelocity();
        FVector forwordVector = Player->GetActorForwardVector();
        Speed = FVector::DotProduct(forwordVector, velocity);

        auto movement = Player->GetCharacterMovement();
        bDodge = movement->IsFalling();
    }
}

void UPlayerAnim::SetRun(bool bValue)
{
    bRun = bValue;
}

void UPlayerAnim::SetDodge(bool bValue)
{
    bDodge = bValue;
}


void UPlayerAnim::OnResetCombo()
{
    Player->ResetCombo();
}


void UPlayerAnim::SetIgnoreAttack(bool bValue)
{
    bIgnoreInputAttack = bValue;
}

void UPlayerAnim::OnEndDamageAnimation()
{
    Player->OnEndDamage();
}

void APlayerCharacter::SetGlobalTimeDilation(bool bSlow)
{
    //bIsSlow != bIsSlow;

    float dilation = bSlow ? 0.5f : 1;
    GetWorldSettings()->SetTimeDilation(dilation);

    //GetWorld()->GetTimerManager().SetTimer(DamageTimerHandler, [this]() {
    //    GetWorldSettings()->SetTimeDilation(1);
    //    }, 1, false);
}
