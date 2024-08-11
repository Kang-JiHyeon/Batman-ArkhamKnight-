// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameModeBase.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "BossMapMainWidget.h"

void APlayerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Main Widget
	MainWidget = Cast<UBossMapMainWidget>(CreateWidget(GetWorld(), MainWidgetFactory));
	MainWidget->AddToViewport(0);
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



