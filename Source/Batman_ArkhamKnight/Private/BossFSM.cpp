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

// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
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

	// fast move
	me->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &UBossFSM::OnMeshBeginOverlap);

	// hp	
	HP = BossHp;

	// GameModeBase
	MyGameModeBase = Cast<APlayerGameModeBase>(GetWorld()->GetAuthGameMode());

	SetCollision(false);
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	case EBossState::DoubleLeftAttack:
		DoubleLeftAttackState();
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
	
	//FString logMsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan,logMsg);
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
	me->AddMovementInput(dir.GetSafeNormal(), 0.1f);
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > moveDelayTime)
	{
		int32 statevalue = FMath::RandRange(0, 12);
		if (statevalue == 0 && MyGameModeBase->IsPlayingSequence()==false)
		{
			direction = Ptarget->GetActorLocation() - me->GetActorLocation();
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
				if (attackstatevalue < 1 && MyGameModeBase->IsPlayingSequence() == false)
				{
					currentTime = 0;
					if (FMath::RandBool()) {
						SetCollision(true);
						mState = EBossState::DoubleRightAttack;
						anim->BanimState = mState;
					}
					else {
						SetCollision(true);
						mState = EBossState::DoubleLeftAttack;
						anim->BanimState = mState;
					}
				}
				else if ((attackstatevalue == 1 || attackstatevalue == 2 ) && MyGameModeBase->IsPlayingSequence() == false)
				{
					currentTime = 0;
					anim->bAttackPlay = true;
					SetCollision(true);
					mState = EBossState::LeftAttack;
					anim->BanimState = mState;
					currentTime = attackDelayTime;
				}
				else if((attackstatevalue == 3 || attackstatevalue == 4) && MyGameModeBase->IsPlayingSequence() == false)
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
	UE_LOG(LogTemp, Warning, TEXT("RightAttack"));
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
	UE_LOG(LogTemp, Warning, TEXT("LeftAttack"));
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
	if (currentTime > 2)
	{
		SetCollision(false);
		currentTime = 0;
		mState = EBossState::Move;
		anim->BanimState = mState;
	}
}

void UBossFSM::DoubleLeftAttackState() // double smash
{
	UE_LOG(LogTemp, Warning, TEXT("DoubleleftAttack"));
	currentTime += GetWorld()->DeltaTimeSeconds;

	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	// double attack 2 type
	if (currentTime > 2)
	{
		SetCollision(false);
		currentTime = 0;
		mState = EBossState::Move;
		anim->BanimState = mState;
	}
}

void UBossFSM::DamageState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > damageDelayTime)
	{
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
	check(CrawlCameraShake);
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

void UBossFSM::OnMyTakeDamage(int32 damage)

{
	if (mState == EBossState::Die)
	{
		return;
	}
	HP -= damage;

	if (HP > 0)
	{
		currentTime = 0;
		mState = EBossState::Damage;
		anim->BanimState = mState;

	}
	else
	{
		SetCollision(false);
		mState = EBossState::Die;
		anim->BanimState = mState;
	}
	UE_LOG(LogTemp, Warning, TEXT("Boss Damage!! : Hp = %d"), HP);

}

void UBossFSM::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// fastmove상태에서 mesh가 overlap되면 player를 밀고 다시 move상태로 돌아온다.
	auto* player = Cast<APlayerCharacter>(OtherActor);
	if (mState == EBossState::Crawl)
	{
		if (player != nullptr)
		{
			// player가 밀리는 함수 추가
			Ptarget->OnTakeDamage(me, 3);
			SetCollision(false);
		}
	}
	else if (mState == EBossState::DoubleLeftAttack || mState == EBossState::DoubleRightAttack ||
		mState == EBossState::LeftAttack || mState == EBossState::RightAttack)
	{
		if (player != nullptr)
		{
			Ptarget->OnTakeDamage(me, 2);
			SetCollision(false);
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