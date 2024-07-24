// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"
#include "SkeletalMesh.h"

// Sets default values
AEnemyBoss::AEnemyBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈메시 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh>bossMesh(TEXT(""));
	// 로드 성공하면
	if (bossMesh)
	{
		GetMesh()->SetSkeletalMesh(bossMesh.Object);
	}
}	

// Called when the game starts or when spawned
void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

