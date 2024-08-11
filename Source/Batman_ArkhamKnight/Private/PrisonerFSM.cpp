// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerFSM.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Prisoner.h"
#include "PrisonerAnim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerGameModeBase.h"

// Sets default values for this component's properties
UPrisonerFSM::UPrisonerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPrisonerFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	Ptarget = Cast<APlayerCharacter>(actor);
	me = Cast<APrisoner>(GetOwner());

	// animation
	anim = Cast<UPrisonerAnim>(me->GetMesh()->GetAnimInstance());

	// HP
	HP = MaxHp;
	
	me->GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &UPrisonerFSM::OnMeshBeginOverlap);
	SetCollision(false);

	// GameModeBase
	MyGameModeBase = Cast<APlayerGameModeBase>(GetWorld()->GetAuthGameMode());
}


// Called every frame
void UPrisonerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FString myState = UEnum::GetValueAsString(mState);
	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), myState, nullptr, FColor::Yellow, 0);
	switch (mState)
	{
	case EPrisonerState::Idle:
		IdleState(DeltaTime);
		break;
	case EPrisonerState::Move:
		MoveState(DeltaTime);
		break;
	case EPrisonerState::Run:
		RunState(DeltaTime);
		break;
	case EPrisonerState::BackMove:
		BackMoveState(DeltaTime);
		break;
	case EPrisonerState::RightAttack:
		RightAttackState(DeltaTime);
		break;
	case EPrisonerState::LeftAttack:
		LeftAttackState(DeltaTime);
		break;
	case EPrisonerState::Damage:
		DamageState(DeltaTime);
		break;
	case EPrisonerState::Faint:
		FaintState(DeltaTime);
		break;
	case EPrisonerState::Die:
		DieState(DeltaTime);
		break;
	}
	//FString logMsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
}

void UPrisonerFSM::SetState(EPrisonerState NextState)
{
	EPrisonerState prevState = mState;
	mState = NextState;
	currentTime = 0;
	switch (mState)
	{
	case EPrisonerState::Idle:
		break;
	case EPrisonerState::Move:
		break;
	case EPrisonerState::Run:
		break;
	case EPrisonerState::BackMove:
		break;
	case EPrisonerState::RightAttack:
		break;
	case EPrisonerState::LeftAttack:
		break;
	case EPrisonerState::Damage:
		break;
	case EPrisonerState::Faint:
		break;
	case EPrisonerState::Die:
		break;
	default:
		break;
	}

}

void UPrisonerFSM::IdleState(float& DeltaSeconds)
{

	currentTime += DeltaSeconds;
	if (currentTime > idleDelayTime)
	{

		SetState(EPrisonerState::BackMove);
		anim->PanimState = mState;

	}
}

void UPrisonerFSM::MoveState(float& DeltaSeconds){

	// Ÿ���� ������
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	// �÷��̾� �������� �̵�
	me->AddMovementInput(dir.GetSafeNormal(), 0.05f);
	currentTime += DeltaSeconds;
	if (currentTime > moveDelayTime)
	{
		if (dir.Size() < 200 && MyGameModeBase->IsPlayingSequence() == true)
		{
			SetState(EPrisonerState::BackMove);
			anim->PanimState = mState;
		}
		else
		{
			int value = FMath::RandRange(0, 100);
			if (value < 40)
			{
				SetState(EPrisonerState::Run);
				anim->PanimState = mState;
			}
			else
			{
				SetState(EPrisonerState::BackMove);
				anim->PanimState = mState;
			}
		}

	}

}

void UPrisonerFSM::RunState(float& DeltaSeconds)
{
	if (MyGameModeBase->IsPlayingSequence() == true)
	{
		SetState(EPrisonerState::BackMove);
		anim->PanimState = mState;
	}
	// �÷��̾ ���� �޸��� �ʹ�.
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	float dist = me->GetDistanceTo(Ptarget);
	me->AddMovementInput(dir.GetSafeNormal(), 0.5f);

	currentTime += DeltaSeconds;
	if (currentTime < 5 ) { // �ִ� 5�ʳ��� player�� ��ó�� ���� ������ 5�ʷ� ����
		if (dist < 100) {
			// ������ ���ݰ� ���� ������ �����ϰ� ������ �ϰ� �ʹ�.
			if (FMath::RandBool()) {
				SetCollision(true);
				SetState(EPrisonerState::RightAttack);
				anim->attack = true;
			}
			else {
				SetCollision(true);
				SetState(EPrisonerState::LeftAttack);
				anim->attack = false;
			}
			anim->PanimState = mState;
		}

	}
	else {
		SetState(EPrisonerState::Move);
		anim->PanimState = mState;

	}
}

void UPrisonerFSM::BackMoveState(float& DeltaSeconds)
{
	// Ÿ���� ������
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	// �ް����� ġ�� �ʹ� = �÷��̾ �ٶ󺸸� �ڷ� �Ȱ� �ʹ�.
	// �������� ȸ�� (���� ���)
	FRotator currentRotation = me->GetActorRotation();
	FRotator targetRotation = dir.Rotation();
	FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaSeconds, 15.0f);
	me->SetActorRotation(newRotation);

	// �÷��̾��� �ݴ� �������� �̵�
	FVector backwardDir = -dir.GetSafeNormal();
	me->AddMovementInput(backwardDir,0.05f);
	currentTime += DeltaSeconds;
	if (currentTime > backmoveDelayTime)
	{
		if (FMath::RandBool())
		{
			SetState(EPrisonerState::Move);
			anim->PanimState = mState;
		}
		else
		{
			SetState(EPrisonerState::Idle);
			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::RightAttackState(float& DeltaSeconds)
{
	// ���ݹ������� �÷��̾ ������ ���� 1�� �Ŀ� �����ϰ� �ʹ�.
	// ������ ��ġ�� �ϰ� �ʹ�.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{
		// ��ġ�� �ϰ� �� �� �ٽ� �̵����� �����ϰ� �ʹ�.
		float dist = me->GetDistanceTo(Ptarget);
		if (dist < attackDistance)
		{
			SetState(EPrisonerState::BackMove);
			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::LeftAttackState(float& DeltaSeconds)
{
	// ���ݹ������� �÷��̾ ������ ���� 1�� �Ŀ� �����ϰ� �ʹ�.
	// ���� ��ġ�� �ϰ� �ʹ�.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{
		// ��ġ�� �ϰ� �� �� �ٽ� �̵����� �����ϰ� �ʹ�.
		float dist = me->GetDistanceTo(Ptarget);
		if (dist < attackDistance)
		{
			SetState(EPrisonerState::BackMove);
			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::DamageState(float& DeltaSeconds)
{

	FVector dir = me->GetActorLocation() - Ptarget->GetActorLocation();
	float dis = dir.Size();
	dir.Normalize();


	if (dis < 120) {
		me->GetCharacterMovement()->Velocity = dir * 2000;
	}

	currentTime += DeltaSeconds;
	if (currentTime > damageDelayTime)
	{
		if (HP/MaxHp < 0.5f && HP/MaxHp >0)
		{
			SetCollision(false);
			SetState(EPrisonerState::Faint);
			anim->PanimState = mState;
		}
		else if (HP < 0)
		{
			SetCollision(false);
			SetState(EPrisonerState::Die);
			anim->PanimState = mState;
		}
		else
		{
			SetState(EPrisonerState::Move);
			anim->PanimState = mState;
		}
	}

}

void UPrisonerFSM::FaintState(float& DeltaSeconds)
{

	SetCollision(false);
	currentTime += DeltaSeconds;
	if (currentTime > FaintDelayTime)
	{

		SetState(EPrisonerState::Move);
		anim->PanimState = mState;
	}
}


void UPrisonerFSM::DieState(float& DeltaSeconds)
{
	
}

void UPrisonerFSM::OnPlayerHit()
{
	float dist = me->GetDistanceTo(Ptarget);
	
	if (Ptarget != nullptr)
	{
		if (dist < attackDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("player is attacked by prisoner"));
			Ptarget->OnTakeDamage(me, 1);
			SetCollision(false);
			SetState(EPrisonerState::BackMove); // ������ ���� ���� back move ���� 
			anim->PanimState = mState;
		}
		else
		{
			SetState(EPrisonerState::Move);
			anim->PanimState = mState;
		}
	}
}

void UPrisonerFSM::OnMyTakeDamage(int32 damage)

// �������� �Դٰ� ���� HP ���ϰ� �Ǹ� �������¿� ���� �ʹ�.
// �������¿� ���� ���� ���� �ð����Ŀ� �ٽ� �̵� ���·� �����ϰ� �ʹ�.
{

	if (mState == EPrisonerState::Die || mState == EPrisonerState::Faint)
	{
		return;
	}
	if (IsAttack() == true)
	{
		HP -= 2*damage;
	}
	else {
		HP -= damage;
	}

	UE_LOG(LogTemp, Warning, TEXT("Prisoner Damage!! : Hp = %f"), HP);
	SetState(EPrisonerState::Damage);
	anim->PanimState = mState;

}

void UPrisonerFSM::SetCollision(bool bvalue)
{
	if (bvalue)
	{
		// collision�� �Ѿ���
		// ��ġ�� �Դ� ����
		// �׷��� �Ϲݻ��¿����� �ٰ����� ƨ��
		me->GetMesh()->SetCollisionProfileName(TEXT("Prisoner"));
	}
	else {
		// collision�� ������
		// ���ٴ� ���� ���� �Ǵ� ���� ���°� �Ǿ� collision�� ���� �ʴ� ����
		me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
// begin play���� false�� �����Ͽ� collision�� ���� ���¿��ٰ�
// damage�� �ް� �����ϰų� ������ true�� �ٲ���Ѵ�
// attack�� �� ���� �Ѿ��ϰ� attack�� ������ �����Ѵ�.


void UPrisonerFSM::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//auto* player = Cast<APlayerCharacter>(OtherActor);

	//if (player != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Overlap with Player detected!"));
	//	Ptarget->OnTakeDamage(me, 1);
	//	SetCollision(false);
	//	SetState(EPrisonerState::BackMove); // ������ ���� ���� back move ���� 
	//	anim->PanimState = mState;
	//}
}

bool UPrisonerFSM::IsAttack()
{
	if (mState == EPrisonerState::LeftAttack || mState == EPrisonerState::RightAttack || mState == EPrisonerState::Run)
	{
		return true;
	}
	else
	{
		return false;
	}
}
