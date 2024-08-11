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
}

void APlayerGameModeBase::StartPlay()
{
    Super::StartPlay();

	// Level Sequence
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bDisableLookAtInput = true;
	Settings.bDisableMovementInput = true;
	Settings.bHideHud = true;

	if (AttackSequence != nullptr)
	{
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), AttackSequence, Settings, SequenceActor);
	}

}

void APlayerGameModeBase::PlaySequence()
{
	if (SequencePlayer == nullptr) return;

	//OnStartedLevelSequence.AddDynamic(this, &);

	OnStartedLevelSequence.Broadcast();
    SequencePlayer->Play();
}

bool APlayerGameModeBase::IsPlayingSequence()
{
	if(SequencePlayer == nullptr) return false;

	return SequencePlayer->IsPlaying();
}

void APlayerGameModeBase::SetPlayerHPBar(const int32 CurrHP, const int32 MaxHP)
{
	MainWidget->UpdatePlayerHPBar(CurrHP, MaxHP);
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
		MainWidget->VisibleOverUI();
	}
}
