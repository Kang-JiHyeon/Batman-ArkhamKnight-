// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "BossFSM.h"
#include "EnemyPlayer.h"
#include "Prisoner.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"


// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	fsm = CreateDefaultSubobject<UBossFSM>(TEXT("FSM"));

	// 애니메이션 블루프린트 할당하기
	ConstructorHelpers::FClassFinder<UAnimInstance> animClass(TEXT("/Script/Engine.AnimBlueprint'/Game/SYH/Blueprints/ABP_SYHBoss.ABP_SYHBoss_C'"));
	if (animClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animClass.Class);
	}

	TailComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailComp"));
	TailComp->SetupAttachment(GetMesh(),TEXT("spine_01"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempTailMesh(TEXT("/Script/Engine.StaticMesh'/Game/SYH/Characters/KillerCroc/TestKiller-Tail.TestKiller-Tail'"));
	if (TempTailMesh.Succeeded())
	{
		TailComp->SetStaticMesh(TempTailMesh.Object);
		TailComp->SetRelativeLocationAndRotation(FVector(-409.198095, 294.424819, 29.297073),FRotator((2.033529, 67.198997, -96.366017)));
	}

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(GetMesh());
	SphereComp->SetRelativeLocation(FVector(0.000009, -170.206918, 743.073657));
	SphereComp->SetRelativeScale3D(FVector(5));
}	

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	FRotator InitialTailRotation;
	FVector InitialTailLocation;
	Super::Tick(DeltaTime);
	if (fsm->mState == EBossState::Crawl)
	{

		// 기어가는 상태에서 꼬리를 회전시키고 싶다.
		TailComp->SetRelativeLocationAndRotation(FVector(0.045342, 381.207363, 9.576032), FRotator(-2.524381, 7.448579, -96.188432));
		time += DeltaTime;

	}
	if (time > 2)
	{
		TailComp->SetRelativeLocationAndRotation(FVector(-409.198095, 294.424819, 29.297073), FRotator(2.033529, 67.198997, -96.366017));
		time = 0;
	}

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super:: NotifyActorBeginOverlap(OtherActor);

	APrisoner* prisoner = Cast<APrisoner>(OtherActor);
	if (prisoner && fsm->mState == EBossState::Crawl)
	{
		FVector velo = prisoner->GetActorLocation() - GetActorLocation();
		velo.Normalize();
		velo *= FVector(1200.0f, 1200.0f, 0.0f); // 각 축에 곱셈

		prisoner->LaunchCharacter(velo, true, false);
		UE_LOG(LogTemp, Warning, TEXT(" Boss NotifyActorBeginOverlap"));
	}
}


