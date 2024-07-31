// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemy.h"
#include "PlayerCharacter.h"

// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	auto* player = Cast<APlayerCharacter>(OtherActor);

	if (player != nullptr)
	{
		player->OnDamageProcess(this, 1);

		UE_LOG(LogTemp, Warning, TEXT("Enemy->Player Attack!!"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Enemy NotifyActorBeginOverlap"));

}

// Called every frame
void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATestEnemy::GetValided()
{
	return bValid;
}

