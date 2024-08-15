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
#include "Components/WidgetComponent.h"
#include "PrisonerAttackWidget.h"

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
	FString myState = UEnum::GetValueAsString(mState);
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), myState, nullptr, FColor::Yellow, 0);
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

void UPrisonerFSM::MoveState(float& DeltaSeconds) {
	if (HP <= 0)
		return;

	// 타깃의 목적지
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	// 플레이어 방향으로 이동
	me->AddMovementInput(dir.GetSafeNormal(), 0.05f);
	currentTime += DeltaSeconds;
	if (currentTime > moveDelayTime)
	{
		if (dir.Size() < 300 && MyGameModeBase->IsPlayingSequence() == true)
		{
			SetState(EPrisonerState::Move);
			anim->PanimState = mState;
		}
		else
		{
			int value = FMath::RandRange(0, 100);
			if (value < 95)
			{
				if (PrisonerScream)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), PrisonerScream);
				}
				me->Visible();
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
	// 플레이어를 향해 달리고 싶다.
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	float dist = me->GetDistanceTo(Ptarget);
	me->AddMovementInput(dir.GetSafeNormal(), 0.7f);

	currentTime += DeltaSeconds;
	if (currentTime < 5) { // 최대 5초내에 player의 근처에 오기 때문에 5초로 설정
		if (dist < 100) {
			currentTime = 0;
			if (PrisonerAttack)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), PrisonerAttack);
			}
			// 오른쪽 공격과 왼쪽 공격을 랜덤하게 나오게 하고 싶다.

			if (FMath::RandBool())
			{
				SetCollision(true);
				SetState(EPrisonerState::RightAttack);
				anim->attack = true;
			}
			else
			{
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
	// 타깃의 목적지
	FVector destination = Ptarget->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	// 뒷걸음질 치고 싶다 = 플레이어를 바라보며 뒤로 걷고 싶다.
	// 방향으로 회전 (보간 사용)
	FRotator currentRotation = me->GetActorRotation();
	FRotator targetRotation = dir.Rotation();
	FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaSeconds, 15.0f);
	me->SetActorRotation(newRotation);

	// 플레이어의 반대 방향으로 이동
	FVector backwardDir = -dir.GetSafeNormal();
	me->AddMovementInput(backwardDir, 0.05f);
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
	// 공격범위내에 플레이어가 들어오고 나서 1초 후에 공격하고 싶다.
	// 오른쪽 펀치를 하고 싶다.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{

		// 펀치를 하고 난 후 다시 이동으로 전이하고 싶다.
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
	// 공격범위내에 플레이어가 들어오고 나서 1초 후에 공격하고 싶다.
	// 왼쪽 펀치를 하고 싶다.
	currentTime += DeltaSeconds;
	if (currentTime > attackDelayTime)
	{

		// 펀치를 하고 난 후 다시 이동으로 전이하고 싶다.
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

	currentTime += DeltaSeconds;
	if (currentTime > anim->damageDelayTime)
	{
		if (HP / MaxHp < 0.5f && HP / MaxHp >0)
		{
			SetCollision(false);
			SetState(EPrisonerState::Faint);
			anim->PanimState = mState;
		}
		else if (HP <= 0)
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
	SetCollision(false);
	// capsule 끄고 mesh의 prsset 변경
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
			SetState(EPrisonerState::BackMove); // 공격이 끝난 후의 back move 상태 
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

// 데미지를 입다가 일정 HP 이하가 되면 기절상태에 들어가고 싶다.
// 기절상태에 들어가고 나서 일정 시간이후에 다시 이동 상태로 전이하고 싶다.
{


	if (mState == EPrisonerState::Die || mState == EPrisonerState::Faint)
	{
		return;
	}
	if (HP <= 0) return;

	HP = FMath::Max(0, HP - damage);

	FVector dir = me->GetActorLocation() - Ptarget->GetActorLocation();
	float dis = dir.Size();
	dir.Normalize();
	me->GetCharacterMovement()->Velocity = dir * 2000 * damage;

	UE_LOG(LogTemp, Warning, TEXT("damage = %d "), damage);


	int value = FMath::RandRange(0, 2);
	UE_LOG(LogTemp, Warning, TEXT("Prisoner Damage!! : Hp = %f"), HP);
	if (value == 0)
	{
		anim->damageInt = 0;
		anim->damage = false;
		if (PrisonerDamageSound1)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PrisonerDamageSound1, me->GetActorLocation(),1,1,0, PrisonerSA);
		}
		if (PrisonerDamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PrisonerDamageSound, me->GetActorLocation(), 1, 1, 0, PrisonerSA);
		}
	}
	else if (value == 1)
	{
		anim->damageInt = 1;
		anim->damage = true;

		if (PrisonerDamageSound2)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PrisonerDamageSound2, me->GetActorLocation(), 1, 1, 0, PrisonerSA);
		}
		if (PrisonerDamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PrisonerDamageSound2, me->GetActorLocation(), 1, 1, 0, PrisonerSA);
		}
	}
	else
	{
		anim->damageInt = 2;
		anim->damage = false;
		if (PrisonerDamageSound3)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PrisonerDamageSound3, me->GetActorLocation(), 1, 1, 0, PrisonerSA);
		}
		if (PrisonerDamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PrisonerDamageSound, me->GetActorLocation(), 1, 1, 0, PrisonerSA);
		}
	}
	if (damage >= 2)
	{
		anim->damageDelayTime = 5.0f;
	}
	else
	{
		anim->damageDelayTime = 1.0f;
	}
	if (HP <= 0)
	{
		// 죽음 상태 진입 처리
		if (APrisoner* Prisoner = Cast<APrisoner>(GetOwner()))
		{
			Prisoner->OnDeathStateEntered();
		}
		SetCollision(false);
		SetState(EPrisonerState::Die);
		anim->PanimState = mState;

		me->SetActorEnableCollision(false);
		me->GetMesh()->SetCollisionProfileName(TEXT("PrisonerDie"));
	}
	else
	{
		SetState(EPrisonerState::Damage);
		anim->PanimState = mState;
	}
}

void UPrisonerFSM::SetCollision(bool bvalue)
{
	if (bvalue)
	{
		// collision을 켜야함
		// 펀치가 먹는 상태
		// 그러나 일반상태에서도 다가가면 튕김
		me->GetMesh()->SetCollisionProfileName(TEXT("Prisoner"));
	}
	else {
		// collision을 꺼야함
		// 끈다는 것은 기절 또는 죽음 상태가 되어 collision이 먹지 않는 상태
		me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
// begin play에서 false로 시작하여 collision이 없는 상태였다가
// damage를 받고 기절하거나 죽으면 true로 바꿔야한다
// attack을 할 때도 켜야하고 attack이 끝나면 꺼야한다.


void UPrisonerFSM::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//auto* player = Cast<APlayerCharacter>(OtherActor);

	//if (player != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Overlap with Player detected!"));
	//	Ptarget->OnTakeDamage(me, 1);
	//	SetCollision(false);
	//	SetState(EPrisonerState::BackMove); // 공격이 끝난 후의 back move 상태 
	//	anim->PanimState = mState;
	//}
}

bool UPrisonerFSM::IsCounter()
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
