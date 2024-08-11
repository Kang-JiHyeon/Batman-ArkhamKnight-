// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FPlayerGameModeBaseOnStartedLevelSequence);

UCLASS()
class BATMAN_ARKHAMKNIGHT_API APlayerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;
    virtual void StartPlay() override;

public:
    FPlayerGameModeBaseOnStartedLevelSequence OnStartedLevelSequence;

private:
	// ���� ������ �ּ�
    UPROPERTY(EditAnywhere, Category = "Camera")
    class ULevelSequence* AttackSequence;

    // ���� ������ �÷��̾�
    UPROPERTY()
    class ULevelSequencePlayer* SequencePlayer;

    // ���� ������ ����
    UPROPERTY()
    class ALevelSequenceActor* SequenceActor;

    // Main Widget
    UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> MainWidgetFactory;

public:
    UPROPERTY()
    class UBossMapMainWidget* MainWidget;

public:
    void PlaySequence();
    bool IsPlayingSequence();
    
    void SetPlayerHPBar(const int32 CurrHP, const int32 MaxHP);

};
