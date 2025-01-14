﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PlayerAnim.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Boss.h"
#include "BossFSM.h"
#include "Prisoner.h"
#include "PrisonerFSM.h"
#include "TimerManager.h"
#include "../../../../Plugins/Animation/MotionWarping/Source/MotionWarping/Public/MotionWarpingComponent.h"
#include "PlayerGameModeBase.h"
#include "PlayerMotionWarping.h"
#include "CheckTargetDirection.h"
#include "BossMapMainWidget.h"
#include "PlayerSoundManager.h"
#include "Components/AudioComponent.h"
#include "PlayerEffectManager.h"
#include "PlayerAntidoteDetector.h"
#include "PlayerCameraShake.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스프링암
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	// 카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// 회전 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// 망토 Static Mesh
    CapeMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CapeMeshComp"));
    CapeMeshComp->SetupAttachment(GetMesh(), TEXT("spine_03"));
    CapeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 모션 워핑 컴포넌트
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
	PlayerMotionWarpingComp = CreateDefaultSubobject<UPlayerMotionWarping>(TEXT("PlayerMotionWarpingComp"));

	// 타겟 방향 확인 컴포넌트
	CheckTargetDirComp = CreateDefaultSubobject<UCheckTargetDirection>(TEXT("CheckTargetDirComp"));

	// 사운드
	SoundManager = CreateDefaultSubobject<UPlayerSoundManager>(TEXT("SoundManager"));
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	// 이펙트
	EffectManager = CreateDefaultSubobject<UPlayerEffectManager>(TEXT("EffectManager"));

	// 해독제 감지
	AntidoteDetector = CreateDefaultSubobject<UPlayerAntidoteDetector>(TEXT("AntidoteDetector"));

	// 카메라 쉐이크
	CameraShake = CreateDefaultSubobject<UPlayerCameraShake>(TEXT("CameraShake"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 컨트롤러를 가져와서 PlayerController인지 캐스팅한다.
	auto* playerController = Cast<APlayerController>(Controller);

	// 캐스팅 성공했다면
	if (playerController != nullptr)
	{
		// 그 객체를 이용해서 EnhancedInputLocalPlayerSubsystem을 가져온다.
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

		// SubSys를 이용하여 AddMappingContext를 한다.
		if (subSys != nullptr)
		{
			subSys->AddMappingContext(IMP_Player, 0);
		}
	}
	// GameModeBase
	MyGameModeBase = Cast<APlayerGameModeBase>(GetWorld()->GetAuthGameMode());

	// 애니메이션
	PlayerAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());

	// Stat 초기화
	SetHP(MaxHP);
	SetHitCombo(0);
	SetSkillCombo(0);

	// 보스
	AActor* boss = UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass());
	TargetBoss = Cast<ABoss>(boss);
	PlayerMotionWarpingComp->OnInitialize(TargetBoss);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 회전 방향으로 이동하고 싶다.
	// 1.ControlRotation을 이용해서 Transform 을 만들고
	FTransform controlTransform = FTransform(GetControlRotation());
	// 2. TransformDirection 기능을 이용해서 방향을 만들어서
	Direction = controlTransform.TransformVector(Direction);
	// 3. 그 방향으로 이동한다.
	AddMovementInput(Direction, 1);
	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnActionMove);
	Input->BindAction(IA_Move, ETriggerEvent::Completed, this, &APlayerCharacter::OnActionMoveCompleted);
	Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::OnActionLook);
	Input->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APlayerCharacter::OnActionDodge);
	Input->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::OnActionAttack);
	Input->BindAction(IA_BossAttack, ETriggerEvent::Started, this, &APlayerCharacter::OnActionBossAttack);


	AntidoteDetector->SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::OnActionMove(const FInputActionValue& Value)
{
	if (IsLockedAction())	return;

	FVector2D v = Value.Get<FVector2D>();

	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Normalize();

	if(bMoveInputPressed == false)
		bMoveInputPressed = true;
}

void APlayerCharacter::OnActionMoveCompleted(const FInputActionValue& Value)
{
	bMoveInputPressed = false;
}

void APlayerCharacter::OnActionLook(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();

    AddControllerPitchInput(-v.Y);
    AddControllerYawInput(v.X);
}

/// <summary>
/// 이동 방향이 있고, 스페이스 바를 2번 누를 경우 구르기를 한다.
/// </summary>
/// <param name="Value"></param>
void APlayerCharacter::OnActionDodge(const FInputActionValue& Value)
{
	if(bMoveInputPressed == false || IsLockedAction()) return;

	float currtime = GetWorld()->GetTimeSeconds();

	if (currtime - LastDodgeInputPressTime <= DoublePressInterval)
	{
		//GetCharacterMovement()->Velocity = GetActorUpVector() * 200;
        //Jump();

		PlayAnimMontage(DodgeMontage);

		// 구르기 시 공격 콤보 초기화
		SetHitCombo(0);	

		// 사운드 재생
		SoundManager->PlaySound(EPlayerSoundType::Dodge);
	}
	LastDodgeInputPressTime = currtime;
}

/// <summary>
/// 기본 공격 함수
/// </summary>
/// <param name="Value"></param>
void APlayerCharacter::OnActionAttack(const FInputActionValue& Value)
{
	if (IsLockedAction())
	{
		//UE_LOG(LogTemp, Warning, TEXT("이미 이동 중이여서 공격 대상으로 이동할 수 없습니다."));
		return;
	}

	bIsSlow = false;

	TargetPrisoner = FindTargetPrisoner();

	// 공격할 대상이 있다면
	if (TargetPrisoner != nullptr)
	{
		bIsSlow = TargetPrisoner->fsm->IsCounter();

		PlayAttackAnimation();
	}
	// 공격할 수 있는 대상이 없다면, 앞방향으로 일정거리만큼 이동
	else
	{
		// 콤보 카운트 증가
		FString section = FString::FromInt((FrontAnimIndex % 3));
		// 애니메이션 실행
		PlayAnimMontage(FrontAttackMontage, 1, FName(*section));
		FrontAnimIndex++;
		// 사운드 재생
		SoundManager->PlaySound(EPlayerSoundType::InvaildAttack);
		
		GetCharacterMovement()->Velocity = GetActorForwardVector() * 20000000;
	}
}

void APlayerCharacter::OnActionBossAttack(const FInputActionValue& Value)
{
	if (TargetBoss == nullptr) return;
	if (TargetBoss->fsm->mState == EBossState::Die) return;
	if(IsLockedAction()) return;
	if(SkillCombo < MaxSkillCombo) return;

	// 몽타주 재생
	PlayAnimMontage(BossAttackMotages[bossAttackIndex]);
	// 시퀀스 재생
	MyGameModeBase->PlaySequence(bossAttackIndex);

	bossAttackIndex = (bossAttackIndex + 1) % BossAttackMotages.Num();
	
	// 보스 공격 콤보 초기화
	SetSkillCombo(0);

	PlayerAnim->SetIgnoreAttack(false);
}

bool APlayerCharacter::IsLockedAction() const
{
	if (MyGameModeBase == nullptr || PlayerAnim == nullptr) return false;
	
	bool bIsMontagePlaying = PlayerAnim->IsAnyMontagePlaying();
	return bDamageState || PlayerAnim->bDodge || bIsMontagePlaying || MyGameModeBase->IsPlayingSequence();
}

APrisoner* APlayerCharacter::FindTargetPrisoner()
{
	TArray<AActor*> targetActors;
	float minDistance = AttackRange;

	// Prisoner를 모두 탐색
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APrisoner::StaticClass(), targetActors);

	// 탐색된 Prisoner가 없을 경우 종료
	if (targetActors.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("탐색된 적이 없습니다."));
		return nullptr;
	}

	//// 기존의 이동 대상을 초기화
	APrisoner* targetPrisoner = nullptr;

	for (AActor* targetActor : targetActors)
	{
		APrisoner* prisoner = Cast<APrisoner>(targetActor);

		if (prisoner == nullptr || prisoner->IsAttackable() == false)
		{
			//UE_LOG(LogTemp, Warning, TEXT("적이 기절 상태입니다. 다음 적을 탐색합니다."));
			continue;
		}

		// Prisoner와 나의 위치의 거리가 현재 탐색된 적과의 최소 거리보다 작다면, Target 대상 변경
		float distance = FVector::Distance(prisoner->GetActorLocation(), GetActorLocation());
		if (distance < minDistance)
		{
			targetPrisoner = prisoner;
			minDistance = distance;
		}
	}

	return targetPrisoner;
}

void APlayerCharacter::PlayAttackAnimation()
{
	if(TargetPrisoner == nullptr) return;

	// 앞, 뒤 방향 확인
	EDirectionType enemyDir = CheckTargetDirComp->GetTargetVerticalDirection(TargetPrisoner);

	///적이 앞에 있다면 콤보 애니메이션 실행
	//if (enemyDir == EDirectionType::Front)
	//{
		// 애니메이션 실행
		int randIdx = FMath::RandRange(0, PrisonerAttackMotages.Num() - 1);
		if (randIdx == prisonerAttackIndex) randIdx++;
		prisonerAttackIndex = randIdx % PrisonerAttackMotages.Num();

		PlayAnimMontage(PrisonerAttackMotages[prisonerAttackIndex]);
	//}
	//// 적이 뒤에 있다면 왼쪽, 오른쪽 구분해서 애니메이션 실행
	//else
	//{
	//	FString dirName = CheckTargetDirComp->GetTargetHorizontalDirection(TargetPrisoner) == EDirectionType::Left ? "Left" : "Right";
	//	PlayAnimMontage(BackAttackMontage, 1, FName(dirName));
	//}

	PlayerMotionWarpingComp->PlayMotionWarpingToTarget(TargetPrisoner, 75);
	// 사운드 재생
	SoundManager->PlaySound(EPlayerSoundType::VaildAttack);

	
}

void APlayerCharacter::OnHitPrisoner()
{
	if(TargetPrisoner == nullptr || TargetPrisoner->fsm == nullptr) return;

	// hit 대상이 거리 내에 있다면 데미지 입히기
	float distance = FVector::Distance(TargetPrisoner->GetActorLocation(), GetActorLocation());
	if (distance < 150)
	{
		// 기본 공격 데미지
		int damage = DefalutHitDamage;
		// 반격 공격 데미지
		if(bIsSlow)
			damage = CounterHitDamage;
		// 콤보 데미지 = 공격 데미지 * 2
		if(HitCombo >= MaxHitCombo)
			damage *= HitComboMultiplier;

		TargetPrisoner->fsm->OnMyTakeDamage(damage);
		OnHitSucceeded(1);

		UE_LOG(LogTemp, Warning, TEXT("Player->Prisoner Damage : %d"), damage);

		// Effect
		EPlayerEffectType effectType = HitCombo < MaxHitCombo ? EPlayerEffectType::DefaultAttack : EPlayerEffectType::SpecialAttack;
		EffectManager->SpawnEffectAtLocation(effectType, TargetPrisoner->GetActorLocation(), TargetPrisoner->GetActorRotation());

		// 카메라 쉐이크
		CameraShake->PlayCameraShake(ECameraShakeType::Attack);
	}
}

void APlayerCharacter::OnHitBoss()
{
	if (TargetBoss == nullptr) return;
	if (CurrAttackType == EAttackType::Run) return;

	TargetBoss->fsm->OnMyTakeDamage(CurrAttackType, 5);

	// 사운드 재생
	SoundManager->PlaySound(EPlayerSoundType::BossAttack);
}

void APlayerCharacter::OnPlayMotionWarping(EAttackType AttackType)
{
	CurrAttackType = AttackType;

	PlayerMotionWarpingComp->AddAndUpdateMotionWarping(CurrAttackType);
}

void APlayerCharacter::OnTakeDamage(AActor* OtherActor, int32 Damage)
{
	if(HP <= 0) return;
	if(IsLockedAction()) return;

	SetHP(HP-Damage);
	
	// 공격 콤보 초기화
	SetHitCombo(0);

	// 슬로우 해제
	bIsSlow = false;

	// Damage 처리
	if (HP > 0)
	{
		bDamageState = true;

		// 사운드 재생
		SoundManager->PlaySound(EPlayerSoundType::Damage);

		// 맞은 방향으로 밀리기
		FVector dir = GetActorLocation() - OtherActor->GetActorLocation();
		dir.Z = 0;
		dir.Normalize();
		GetCharacterMovement()->Velocity = dir * 2000;

		// 적이 앞에서 공격했다면, 뒤로 휘청거리기
		EDirectionType dirState = CheckTargetDirComp->GetTargetVerticalDirection(OtherActor);
		if (dirState == EDirectionType::Front)
		{
			PlayAnimMontage(DamageMontage, 1, FName("FrontDamage"));
			//UE_LOG(LogTemp, Warning, TEXT("적이 [앞]에서 때렸습니다!!"));
		}
		// 적이 뒤에서 공격했다면, 앞으로 휘청거리기
		else
		{
			PlayAnimMontage(DamageMontage, 1, FName("BackDamage"));
			//UE_LOG(LogTemp, Warning, TEXT("적이 [뒤]에서 때렸습니다!!"));
		}
		// 일정 시간 뒤 Damage 상태 해제
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandler, this, &APlayerCharacter::OnEndDamage, DamageIdleTime, false);

		// UI 애니메이션 재생
		MyGameModeBase->MainWidget->BlinkRedAllUI();

		// 카메라 쉐이크
		CameraShake->PlayCameraShake(ECameraShakeType::Damage);
	}
	// Die 처리
	else
	{
		bDamageState = true;
		PlayerAnim->bDie = true;
		// 사운드 재생
		SoundManager->PlaySound(EPlayerSoundType::Die);
		//// 콜리전 끄기
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		UE_LOG(LogTemp, Warning, TEXT("Player Die!!"), HP);
	}
}

void APlayerCharacter::OnEndDamage()
{
	bDamageState = false;
	PlayerAnim->bIgnoreInputAttack = false;
}

void APlayerCharacter::OnEndDie()
{
	if (MyGameModeBase != nullptr)
		MyGameModeBase->MainWidget->ShowGameOverWidget(true);
}

void APlayerCharacter::SetHP(float Value)
{
	HP = Value;

	if (MyGameModeBase != nullptr)
		MyGameModeBase->MainWidget->UpdatePlayerHPBar(HP, MaxHP);
}

void APlayerCharacter::SetHitCombo(float Value)
{
	HitCombo = Value;

	if(MyGameModeBase != nullptr)
		MyGameModeBase->MainWidget->UpdatePlayerHitCombo(HitCombo, MaxHitCombo);

}

void APlayerCharacter::SetSkillCombo(float Value)
{
	SkillCombo = Value;

	if (MyGameModeBase != nullptr)
		MyGameModeBase->MainWidget->UpdatePlayerSkillGauge(SkillCombo, MaxSkillCombo);
}

void APlayerCharacter::OnHitSucceeded(float Value)
{
	SetHitCombo(HitCombo + Value);
	SetSkillCombo(SkillCombo + Value);

}
