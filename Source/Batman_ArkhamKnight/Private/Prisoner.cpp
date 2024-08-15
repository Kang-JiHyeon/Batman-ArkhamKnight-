// Fill out your copyright notice in the Description page of Project Settings.


#include "Prisoner.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PrisonerFSM.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerGameModeBase.h"
#include "Components/WidgetComponent.h"
#include "PrisonerAttackWidget.h"
#include "TimerManager.h"

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

	AttackComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("AttackComp"));
	AttackComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FClassFinder<UPrisonerAttackWidget> TempAttackUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SYH/UI/WBP_PrisonerAttack.WBP_PrisonerAttack_C'"));
	if (TempAttackUI.Succeeded())
	{
		AttackComp->SetWidgetClass(TempAttackUI.Class);
	}
}

void APrisoner::OnDeathStateEntered()
{
	APlayerGameModeBase* GameMode = Cast<APlayerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->NotifyEnemyDeath();
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

bool APrisoner::IsAttackable()
{
	if (fsm->mState == EPrisonerState::Faint || fsm->mState == EPrisonerState::Die || fsm->mState == EPrisonerState::Damage)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

void APrisoner::Visible()
{
	AttackComp->SetVisibility(true);
	GetWorld()->GetTimerManager().SetTimer(TimeHandle,this, &APrisoner::Hide, 2.0f, false);
}

void APrisoner::Hide()
{
	AttackComp->SetVisibility(false);
}


