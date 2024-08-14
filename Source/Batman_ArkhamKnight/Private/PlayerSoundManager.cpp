// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSoundManager.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UPlayerSoundManager::UPlayerSoundManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerSoundManager::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerSoundManager::PlaySound(EPlayerSoundType SoundType)
{
	if (Player == nullptr || Player->AudioComp == nullptr) return;

	Player->AudioComp->Stop();

	switch (SoundType)
	{
	case EPlayerSoundType::Move:
		break;
	case EPlayerSoundType::Dodge:
		PlayDodge();
		break;
	case EPlayerSoundType::VaildAttack:
		PlayValidAttack();
		break;
	case EPlayerSoundType::InvaildAttack:
		PlayInvalidAttack();
		break;
	case EPlayerSoundType::BossAttack:
		PlayBossAttack();
		break;
	case EPlayerSoundType::Damage:
		PlayDamage();
		break;
	case EPlayerSoundType::Die:
		PlayDie();
		break;
	default:
		break;
	}

	if(Player->AudioComp->GetSound() != nullptr)
		Player->AudioComp->Play(StartTime);
}

void UPlayerSoundManager::PlayDodge()
{
	SetSoundBase(DodgeSound);
	StartTime = 0;
}

void UPlayerSoundManager::PlayValidAttack()
{
	ValidAttackIdx = GetRandomIndex(0, ValidAttackSounds.Num(), ValidAttackIdx);
	
	SetSoundBase(ValidAttackSounds[ValidAttackIdx]);
	StartTime = 0.3f;
}

void UPlayerSoundManager::PlayInvalidAttack()
{
	InvalidAttackIdx = GetRandomIndex(0, InvalidAttackSounds.Num(), InvalidAttackIdx);

	SetSoundBase(InvalidAttackSounds[InvalidAttackIdx]);
	StartTime = 0.3f;
}

void UPlayerSoundManager::PlayBossAttack()
{
	BossAttackIdx = GetRandomIndex(0, BossAttackSounds.Num(), BossAttackIdx);

	SetSoundBase(BossAttackSounds[BossAttackIdx]);
	StartTime = 0.5f;
}

void UPlayerSoundManager::PlayDamage()
{
	for (auto sound : DamageSounds)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), sound);
	}
	
	SetSoundBase(nullptr);
}

void UPlayerSoundManager::PlayDie()
{
	SetSoundBase(DieSound);
	StartTime = 0;
}

int32 UPlayerSoundManager::GetRandomIndex(int32 Min, int32 Max, int32 Curr)
{
	int32 randIdx = FMath::RandRange(Min, Max - 1);

	if (randIdx == Curr)
		randIdx++;

	return randIdx % Max;
}

void UPlayerSoundManager::SetSoundBase(USoundBase* SoundBase)
{
	Player->AudioComp->SetSound(SoundBase);
}

