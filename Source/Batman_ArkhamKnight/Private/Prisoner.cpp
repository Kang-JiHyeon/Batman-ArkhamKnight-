// Fill out your copyright notice in the Description page of Project Settings.


#include "Prisoner.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PrisonerFSM.h"
#include "Animation/AnimInstance.h"

// Sets default values
APrisoner::APrisoner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	fsm = CreateDefaultSubobject<UPrisonerFSM>(TEXT("FSM"));

	// 애니메이션 블루프린트 할당하기
	ConstructorHelpers::FClassFinder<UAnimInstance> animClass(TEXT("/Script/Engine.AnimBlueprint'/Game/SYH/Blueprints/ABP_SYHPrisoner.ABP_SYHPrisoner_C'"));
	if (animClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animClass.Class);
	}
}

// Called when the game starts or when spawned
void APrisoner::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void APrisoner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APrisoner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool APrisoner::Attackable()
{
	if (fsm->mState == EPrisonerState::Faint || fsm->mState == EPrisonerState::Die)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}


