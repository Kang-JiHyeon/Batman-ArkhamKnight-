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
	//Ÿ���� ������
	FVector destination = Ptarget->GetActorLocation();
	//����
	FVector dir = destination - me->GetActorLocation();
	//�������� �̵�
	me->AddMovementInput(dir.GetSafeNormal(), 0.1f);
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > moveDelayTime)
	{
		int32 statevalue = FMath::RandRange(0, 10);
		if (statevalue == 0 && MyGameModeBase->IsPlayingSequence()==false)
		{
			direction = Ptarget->GetActorLocation() - me->GetActorLocation();
			SetCollision(true);
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


	// prisoner�� player���� ������ ���������� �� �ݴ� ���µ� �ʿ�

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

void UBossFSM::DoubleLeftAttackState() // double smash
{
	UE_LOG(LogTemp, Warning, TEXT("DoubleleftAttack"));
	currentTime += GetWorld()->DeltaTimeSeconds;

	float distance = FVector::Distance(Ptarget->GetActorLocation(), me->GetActorLocation());

	// double attack 2 type
	if (currentTime > 2.3)
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
	if (currentTime >= damageDelayTime)
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
	if (HP < 0) HP = 0;

	if (HP > 0)
	{
		currentTime = 0;
		SetCollision(false);
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

}

void UBossFSM::OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("sphere collision"));

	auto* player = Cast<APlayerCharacter>(OtherActor);
	if (mState == EBossState::Crawl)
	{
		if (player != nullptr)
		{
			// player�� �и��� �Լ� �߰�
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
		// collision�� �Ѿ���
		// ��ġ�� �Դ� ����
		// �׷��� �Ϲݻ��¿����� �ٰ����� ƨ��
		me->GetMesh()->SetCollisionProfileName(TEXT("Boss"));
	}
	else {
		// collision�� ������
		// ���ٴ� ���� ���� �Ǵ� ���� ���°� �Ǿ� collision�� ���� �ʴ� ����
		me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UBossFSM::OnPlayerHit()
{
	float dist = me->GetDistanceTo(Ptarget);

	if (Ptarget != nullptr)
	{
		if (dist < 150)
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