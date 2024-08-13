// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MovieSceneSequencePlayer.h"
#include "PlayerGameModeBase.generated.h"

USTRUCT()
struct FSkillLevelSequence
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY()
    class ULevelSequencePlayer* SequencePlayer;
    UPROPERTY()
    class ALevelSequenceActor* SequenceActor;
    UPROPERTY(EditAnywhere)
    class ULevelSequence* LevelSequence;
};

DECLARE_MULTICAST_DELEGATE(FPlayerGameModeBaseOnStartedLevelSequence);

UCLASS()
class BATMAN_ARKHAMKNIGHT_API APlayerGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
private:
    FTimerHandle VisibleUITimeHandle;
	
public:
    virtual void BeginPlay() override;

public:
    FPlayerGameModeBaseOnStartedLevelSequence OnStartedLevelSequence;

private:

    // Level Sequence
    FMovieSceneSequencePlaybackSettings Settings;

    UPROPERTY(EditDefaultsOnly)
    TArray<FSkillLevelSequence> SkillLevelSequences;


    // Main Widget
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UUserWidget> MainWidgetFactory;


public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    class UBossMapMainWidget* MainWidget;

    UFUNCTION()
    void NotifyEnemyDeath();
    int32 TotalEnemies;
    int32 DeadEnemies;

    void CheckAllEnemiesDead();


private:
    void CreateLevelSequencePlayer();

public:
    void PlaySequence(int32 Index);
    bool IsPlayingSequence();

    void VisibleUI();


};