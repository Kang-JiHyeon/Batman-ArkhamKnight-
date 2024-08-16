// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
//#include "EnemyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Boss.h"
#include "BossAnim.h"
#include "PlayerCharacter.h"
#include "EnemyPlayer.h"
#include "Prisoner.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerGameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "BossHP.h"
#include "PlayerAttackPointComponent.h"
#include "BossMapMainWidget.h"
#include "Components/CapsuleComponent.h"
#include "SYHSoundManager.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	Ptarget = Cast<APlayerCharacter>(actor);
	me = Cast<ABoss>(GetOwner());

	// animation
	anim = Cast<UBossAnim>(me->GetMesh()->GetAnimInstance());

	// attack
	me->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &UBossFSM::OnMeshBeginOverlap);

	// crawl
	me->SphereComp->OnComponentBeginOverlap.AddDynamic(this, &UBossFSM::OnSphereCollisionBeginOverlap);

	auto pri = UGameplayStatics::GetActorOfClass(GetWorld(), APrisoner::StaticClass());
	prisoner = Cast<APrisoner>(pri);
	// GameModeBase
	MyGameModeBase = Cast<APlayerGameModeBase>(GetWorld()->GetAuthGameMode());

	SetCollision(false);

	MyGameModeBase->OnStartedLevelSequence.AddUObject(this, &UBossFSM::SetupBossStateIdle);

	// hp	
	HP = BossHp;
	MyGameModeBase->MainWidget->UpdateBossHPBar(1, 1);


	if (soundmanager)
	{
		soundmanager->PlayBossFirstSound();
		MyGameModeBase->MainWidget->ShowSubtitle1();
		UE_LOG(LogTemp, Warning, TEXT("first"));
	}

}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
	switch (mState)
	{
	case EBossState::Idle:
		IdleState();
		break;
	case EBossState::Move:
		MoveState();
		break;
	case EBossState::RightAttack:
		RightAttackState();
		break;
	case EBossState::LeftAttack:
		LeftAttackState();
		break;
	case EBossState::DoubleRightAttack:
		DoubleRightAttackState();
		break;
	case EBossState::Damage:
		DamageState();
		break;
	case EBossState::Die:
		DieState();
		break;
	case EBossState::Crawl:
		CrawlState();
		break;
	case EBossState::Yell:
		YellState();
		break;
	}
	if (mState == EBossState::Move )
	{
		// 이때 보스 나레이션
		int32 nar = FMath::RandRange(0,100);
		if (nar == 0 && nara == 1)
		{
			if (soundmanager)
			{
				soundmanager->PlayBossSecondSound();
				MyGameModeBase->MainWidget->ShowSubtitle2();
				UE_LOG(LogTemp, Warning, TEXT("second"));
				nara = 2;
			}
		}
		else if (nar == 1 && nara==2)
		{
			if (soundmanager)
			{
				soundmanager->PlayBossThirdSound();
				MyGameModeBase->MainWidget->ShowSubtitle3();
				UE_LOG(LogTemp, Warning, TEXT("third"));
				nara = 3;
			}
		}
	}
	else if (mState == EBossState::LeftAttack || mState == EBossState::RightAttack || mState == EBossState::DoubleRightAttack || mState == EBossState::Crawl)
	{
		// 이때 죄수 나레이션
		int32 nar = FMath::RandRange(0, 100);
		if (nar == 0)
		{
			if (soundmanager && nara ==3)
			{
				soundmanager->PlayPrisonerFirstSound();
				MyGameModeBase->MainWidget->ShowSubtitle4();
				nara = 4;
				UE_LOG(LogTemp, Warning, TEXT("fourth"));
			}
		}
		else if (nar == 1)
		{
			if (soundmanager && nara == 4)
			{
				soundmanager->PlayPrisonerSecondSound();
				MyGameModeBase->MainWidget->ShowSubtitle5();
				nara = 5;
				UE_LOG(LogTemp, Warning, TEXT("fifth"));
			}
		}
		else if (nar == 2 && nara == 5)
		{
			if (soundmanager)
			{
				soundmanager->PlayPrisonerThirdSound();
				MyGameModeBase->MainWidget->ShowSubtitle6();
				nara = 6;
				UE_LOG(LogTemp, Warning, TEXT("six"));
			}
		}
	}

}

void UBossFSM::IdleState()
{
	
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{

		mState = EBossState::Move;
		currentTime = 0;
		anim->BanimState = mState;
	}
}

void UBossFSM::MoveState() // boss move to player or idle
{

	//타깃의 목적지
	FVector destination = Ptarget->GetActorLocation();
	//방향
	FVector dir = destination - me->GetActorLocation();
	//방향으로 이동
	me->AddMovementInput(dir.GetSafeNormal(), 0.2f);
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > moveDelayTime)
	{
		int32 statevalue = FMath::RandRange(0, 10);
		if (statevalue == 0 && MyGameModeBase->IsPlayingSequence()==false)
		{
			if (BossRoarSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), BossRoarSound);
			}
			direction = Ptarget->GetActorLocation() - me->GetActorLocation();
			SetCollision(true);
			me->Visible();
			mState = EBossState::Yell;
			anim->BanimState = mState;
		}
		else if (statevalue > 0 && statevalue <2 && MyGameModeBase->IsPlayingSequence() == false)
		{
			mState = EBossState::Move;
			anim->BanimState = mState;
		}
		else  {
			if (dir.Size() < attackRange)
			{
				int32 attackstatevalue = FMath::RandRange(0, 4);
				if (attackstatevalue == 0 && MyGameModeBase->IsPlayingSequence() == false)
				{
					currentTime = 0;
					SetCollision(true);
					mState = EBossState::DoubleRightAttack;
					anim->BanimState = mState;
				}
				else if ((attackstatevalue == 1 || attackstatevalue == 2) && MyGameModeBase->IsPlayingSequence() == false)
				{
					
					currentTime = 0;
					anim->bAttackPlay = true;
					SetCollision(true);
					mState = EBossState::LeftAttack;
					anim->BanimState = mState;
					currentTime = attackDelayTime;
				}
				else if ((attackstatevalue == 3 || attackstatevalue == 4) && MyGameModeBase->IsPlayingSequence() == false)
				{
					
					currentTime = 0;
					anim->bAttackPlay = true;
					SetCollision(true);
					mState = EBossState::RightAttack;
					anim->BanimState = mState;
					currentTime = attackDelayTime;
				}
			}

		}
		currentTime = 0;
	}


	// prisoner가 player보다 가깝고 기절상태일 때 줍는 상태도 필요

}

void UBossFSM::RightAttackState() // smash
{
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());
	currentTime += GetWorld()->DeltaTimeSeconds;
	anim->bAttackPlay = true;

	if (currentTime > attackDelayTime)
	{
		SetCollision(false);
		mState = EBossState::Move;
		anim->BanimState = mState;
		currentTime = 0;
		anim->bAttackPlay = false;
	}
}


void UBossFSM::LeftAttackState() // smash
{
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	currentTime += GetWorld()->DeltaTimeSeconds;
	anim->bAttackPlay = true;
	if (currentTime > attackDelayTime)
	{
		SetCollision(false);
		mState = EBossState::Move;
		anim->BanimState = mState;
		currentTime = 0;
		anim->bAttackPlay = false;
	}
	
}

void UBossFSM::DoubleRightAttackState() // double smash
{
	UE_LOG(LogTemp, Warning, TEXT("DoubleRightAttack"));
	currentTime += GetWorld()->DeltaTimeSeconds;

	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	// double attack 2 type
	if (currentTime > 2.5)
	{
		SetCollision(false);
		currentTime = 0;
		mState = EBossState::Move;
		anim->BanimState = mState;
	}
}



void UBossFSM::DamageState()
{
	//me->GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	if (HP <= 0)
	{
		// 죽음 상태 진입 처리
		if(prisoner)
		{
			prisoner->OnDeathStateEntered();
		}
		SetCollision(false);
		mState = EBossState::Die;
	}
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime >= damageDelayTime)
	{
		//me->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Boss"));
		mState = EBossState::Move;
		anim->BanimState = mState;
		currentTime = 0;
	}
}

void UBossFSM::DieState()
{
}

void UBossFSM::YellState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > 1.4 && MyGameModeBase->IsPlayingSequence() == false)
	{
		mState = EBossState::Crawl;
		anim->BanimState = mState;
		currentTime = 0;
	}
}



void UBossFSM::CrawlState()
{
// player first position remember and go to there
// if player is there in boss root -> damage
	SetCollision(true);
	if (CrawlCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CrawlCameraShake);
	}
	me->AddMovementInput(direction.GetSafeNormal(), 1.0f);
	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	currentTime += GetWorld()->GetDeltaSeconds();

	if (currentTime >= fastDelayTime)
	{
		mState = EBossState::Move;
		anim->BanimState = mState;
		currentTime = 0;
	}
}

void UBossFSM::SetupBossStateIdle()
{
	mState = EBossState::Idle;
	anim->BanimState = mState;
}


void UBossFSM::OnMyTakeDamage(EAttackType attacktype,int32 damage)

{
	if (mState == EBossState::Die)
	{
		return;
	}
	if (BossDamageSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BossDamageSound);
	}
	HP -= damage;
	//MyGameModeBase->MainWidget->SetRedUI();
	MyGameModeBase->MainWidget->BlinkRedUI();
	MyGameModeBase->MainWidget->UpdateBossHPBar(HP, BossHp);

	UE_LOG(LogTemp, Warning, TEXT("Boss Damage!! : Hp = %d"), HP);

	currentTime = 0;
	SetCollision(false);
	mState = EBossState::Damage;
	anim->attacktype = attacktype;
	anim->BanimState = mState;
	UE_LOG(LogTemp, Warning, TEXT("attack type : %d, mstate : %d"), (int32)attacktype, (int32)mState);

}

void UBossFSM::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void UBossFSM::OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("sphere collision"));

	auto* player = Cast<APlayerCharacter>(OtherActor);
	if (mState == EBossState::Crawl)
	{
		if (player != nullptr)
		{
			// player가 밀리는 함수 추가
			Ptarget->OnTakeDamage(me, 3);
			SetCollision(false);
			mState = EBossState::Move;
			anim->BanimState = mState;
		}
	}
}

void UBossFSM::SetCollision(bool bvalue)
{
	if (bvalue)
	{
		// collision을 켜야함
		// 펀치가 먹는 상태
		// 그러나 일반상태에서도 다가가면 튕김
		me->GetMesh()->SetCollisionProfileName(TEXT("Boss"));
	}
	else {
		// collision을 꺼야함
		// 끈다는 것은 기절 또는 죽음 상태가 되어 collision이 먹지 않는 상태
		me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UBossFSM::OnPlayerHit()
{
	float dist = me->GetDistanceTo(Ptarget);

	if (Ptarget != nullptr)
	{
		if (dist < 250)
		{
			UE_LOG(LogTemp, Warning, TEXT("player is attacked by boss"));
			Ptarget->OnTakeDamage(me, 2);
			SetCollision(false);
		}
		else
		{
			mState = EBossState::Move;
			anim->BanimState = mState;
		}
	}
}