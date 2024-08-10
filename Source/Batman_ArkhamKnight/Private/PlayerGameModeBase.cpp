// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameModeBase.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

void APlayerGameModeBase::StartPlay()
{
    Super::StartPlay();

	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bDisableLookAtInput = true;
	Settings.bDisableMovementInput = true;
	Settings.bHideHud = true;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), AttackSequence, Settings, SequenceActor);
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



