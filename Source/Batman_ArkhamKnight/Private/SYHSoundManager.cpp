// Fill out your copyright notice in the Description page of Project Settings.


#include "SYHSoundManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASYHSoundManager::ASYHSoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASYHSoundManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASYHSoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASYHSoundManager::PlayBossFirstSound()
{
	if (BossFirstSound && Play1 == true)
	{
		UGameplayStatics::PlaySound2D(GetWorld(),BossFirstSound);
		Play1 = false;
	}

}

void ASYHSoundManager::PlayBossSecondSound()
{
	if (BossSecondSound && Play2 == true && Play1 == false)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BossSecondSound);
	}
	Play2 = false;

}

void ASYHSoundManager::PlayBossThirdSound()
{
	if (BossThirdSound && Play3 == true && Play1 == false && Play2 == false)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BossThirdSound);
	}
	Play3 = false;

}

void ASYHSoundManager::PlayPrisonerFirstSound()
{
	if (PrisonerFirstSound && Play4 == true)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PrisonerFirstSound);
	}
	Play4 = false;
}

void ASYHSoundManager::PlayPrisonerSecondSound()
{
	if (PrisonerSecondSound && Play5 == true)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PrisonerSecondSound);
	}
	Play5 = false;
}

void ASYHSoundManager::PlayPrisonerThirdSound()
{
	if (PrisonerThirdSound && Play6 == true)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PrisonerThirdSound);
	}
	Play6 = false;
}

