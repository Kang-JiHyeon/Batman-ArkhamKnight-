// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBossAttackSequence.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerBossAttackSequence::UPlayerBossAttackSequence()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerBossAttackSequence::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bDisableLookAtInput = true;
	Settings.bDisableMovementInput = true;
	Settings.bHideHud = true;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), AttackSequence, Settings, SequenceActor);

}


// Called every frame
void UPlayerBossAttackSequence::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...


}

void UPlayerBossAttackSequence::PlaySequence()
{
	SequencePlayer->Play();

	UE_LOG(LogTemp, Warning, TEXT("Play Sequence!!"));
}

