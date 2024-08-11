// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerAttackPointComponent.h"

void UPlayerAnim::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    auto ownerPawn = TryGetPawnOwner();
    Player = Cast<APlayerCharacter>(ownerPawn);
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    //auto ownerPawn = TryGetPawnOwner();
    //Player = Cast<APlayerCharacter>(ownerPawn);

    if (Player != nullptr)
    {
        FVector velocity = Player->GetVelocity();
        FVector forwordVector = Player->GetActorForwardVector();
        Speed = FVector::DotProduct(forwordVector, velocity);

        auto movement = Player->GetCharacterMovement();
        bDodge = movement->IsFalling();

        //UE_LOG(LogTemp, Warning, TEXT("bDodge : %d"), bDodge);
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
    //Player->ResetCombo();
}


void UPlayerAnim::SetIgnoreAttack(bool bValue)
{
    bIgnoreInputAttack = bValue;
}

void UPlayerAnim::OnEndDamageAnimation()
{
    Player->OnEndDamage();
}

void UPlayerAnim::OnStartSlowMotion()
{
    if(Player->bIsSlow == false) return;

    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);

}

void UPlayerAnim::OnEndSlowMotion()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void UPlayerAnim::AnimNotify_OnHit()
{
    if(Player == nullptr) return;

    Player->OnHitPrisoner();
}

void UPlayerAnim::AnimNotify_OnHitBoss()
{
    if (Player == nullptr) return;

    Player->OnHitBoss();
}

void UPlayerAnim::AnimNotify_OnPlayFlyingKneePunch()
{
    Player->OnPlayMotionWarping(EAttackType::FlyingKneePunch);
}

void UPlayerAnim::AnimNotify_OnPlayDropKick()
{
    Player->OnPlayMotionWarping(EAttackType::DropKick);
}

void UPlayerAnim::AnimNotify_OnPlayFlipKick()
{
    Player->OnPlayMotionWarping(EAttackType::FilpKick);
}
void UPlayerAnim::AnimNotify_OnPlayRun()
{
    Player->OnPlayMotionWarping(EAttackType::Run);
}
