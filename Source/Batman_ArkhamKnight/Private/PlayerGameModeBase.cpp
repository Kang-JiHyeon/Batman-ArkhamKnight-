// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameModeBase.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "BossMapMainWidget.h"
#include "Prisoner.h"

void APlayerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Main Widget
	MainWidget = Cast<UBossMapMainWidget>(CreateWidget(GetWorld(), MainWidgetFactory));
	MainWidget->AddToViewport(0);

	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APrisoner::StaticClass(), FoundEnemies);
	DeadEnemies = 0;
	TotalEnemies = FoundEnemies.Num()+1;

	// Level Sequence
	Settings.bDisableLookAtInput = true;
	Settings.bDisableMovementInput = true;
	Settings.bHideHud = true;
	CreateLevelSequencePlayer();
}

void APlayerGameModeBase::CreateLevelSequencePlayer()
{
	if (SkillLevelSequences.Num() <= 0) return;
	
	for (auto& sequence : SkillLevelSequences)
	{
		if (sequence.LevelSequence == nullptr) continue;

		sequence.SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequence.LevelSequence, Settings, sequence.SequenceActor);
	}
}

void APlayerGameModeBase::PlaySequence(int32 Index)
{
	if (SkillLevelSequences.Num() <= 0 || SkillLevelSequences.Num() <= Index) return;
	if (SkillLevelSequences[Index].SequencePlayer == nullptr) return;

	SkillLevelSequences[Index].SequencePlayer->Play();
	OnStartedLevelSequence.Broadcast();
}

bool APlayerGameModeBase::IsPlayingSequence()
{
	if (SkillLevelSequences.Num() <= 0) return false;

	for (auto& sequence : SkillLevelSequences)
	{
		if (sequence.SequencePlayer != nullptr)
		{
			if (sequence.SequencePlayer->IsPlaying())
				return true;
		}	
	}
	return false;
}

void APlayerGameModeBase::NotifyEnemyDeath()
{
	DeadEnemies++;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Died. DeadEnemies: %d"), DeadEnemies);  // 디버그 로그 추가
	CheckAllEnemiesDead();
}

void APlayerGameModeBase::CheckAllEnemiesDead()
{
	if (DeadEnemies >= TotalEnemies)
	{
		UE_LOG(LogTemp, Warning, TEXT("All enemies are dead. Game Over!"));
		GetWorld()->GetTimerManager().SetTimer(VisibleUITimeHandle, this, &APlayerGameModeBase::VisibleUI, 5.0f, false);
	}
}

void APlayerGameModeBase::VisibleUI()
{
	MainWidget->VisibleOverUI();
}