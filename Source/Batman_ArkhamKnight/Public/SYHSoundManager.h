// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SYHSoundManager.generated.h"

UCLASS(Blueprintable)
class BATMAN_ARKHAMKNIGHT_API ASYHSoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASYHSoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BossFirstSound;
	UPROPERTY(EditAnywhere)
	bool Play1=true;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BossSecondSound;
	UPROPERTY(EditAnywhere)
	bool Play2 = true;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BossThirdSound;
	UPROPERTY(EditAnywhere)
	bool Play3 = true;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PrisonerFirstSound;
		UPROPERTY(EditAnywhere)
	bool Play4 = true;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PrisonerSecondSound;
		UPROPERTY(EditAnywhere)
	bool Play5 = true;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PrisonerThirdSound;
		UPROPERTY(EditAnywhere)
	bool Play6 = true;

	UFUNCTION()
	void PlayBossFirstSound();
	UFUNCTION()
	void PlayBossSecondSound();

	UFUNCTION()
	void PlayBossThirdSound();

	UFUNCTION()
	void PlayPrisonerFirstSound();

	UFUNCTION()
	void PlayPrisonerSecondSound();

	UFUNCTION()
	void PlayPrisonerThirdSound();

};
