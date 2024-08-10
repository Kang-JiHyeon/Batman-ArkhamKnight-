// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PlayerAnim.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TestEnemy.h"
#include "Boss.h"
#include "BossFSM.h"
#include "Prisoner.h"
#include "PrisonerFSM.h"
#include "TimerManager.h"
#include "../../../../Plugins/Animation/MotionWarping/Source/MotionWarping/Public/MotionWarpingComponent.h"
#include "PlayerGameModeBase.h"
#include "PlayerMotionWarpingComponent.h"

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
	CapeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CapeMeshComp"));
	//CapeMeshComp->SetupAttachment(GetMesh(), TEXT("neck_01"));
	CapeMeshComp->SetupAttachment(GetMesh(), TEXT("spine_03"));
	CapeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> CapeMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/KJH/Models/Batman_cape/Batman_Cape.Batman_Cape'"));
	if (CapeMeshFinder.Succeeded())
	{
		CapeMeshComp->SetStaticMesh(CapeMeshFinder.Object);
	}

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
	PlayerMotionWarpingComp = CreateDefaultSubobject<UPlayerMotionWarpingComponent>(TEXT("PlayerMotionWarpingComp"));
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

	// 기본 이동 속도 캐싱
	DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// 매쉬 콜리전 비활성화
	SetMeshCollisionEnabled(false);

	// 매쉬 충돌 이벤트 설정
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnMeshBeginOverlap);

	// HP 초기화
	HP = MaxHP;

	// 보스
	AActor* boss = UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass());
	TargetBoss = Cast<ABoss>(boss);

//	// 타켓의 위치에서 150 앞에 있는 위치
//// 이동할 위치 설정
//	FVector offset = UKismetMathLibrary::GetDirectionUnitVector(TargetBoss->GetActorLocation(), GetActorLocation()) * 150;
//	FVector targetLoc = TargetBoss->GetActorLocation() + offset;
//	// 회전 설정
//	FVector targetDir = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetBoss->GetActorLocation());
//	FRotator targetRot = UKismetMathLibrary::MakeRotFromX(targetDir);
//	// 모션 워핑 실행
//	MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("AttactPoint"), targetLoc, targetRot);
	
	PlayerMotionWarpingComp->OnInitialize(TargetBoss);

	//PlayerMotionWarpingComp->AddMotionWarping(EWarpingPoint::Origin);
	//PlayerMotionWarpingComp->AddMotionWarping(EWarpingPoint::Front);
	//PlayerMotionWarpingComp->AddMotionWarping(EWarpingPoint::Right);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMovingToTarget)
	{
		MoveToTarget(TargetPrisoner);
	}
	else
	{
		// 회전 방향으로 이동하고 싶다.
		// 1.ControlRotation을 이용해서 Transform 을 만들고
		FTransform controlTransform = FTransform(GetControlRotation());
		// 2. TransformDirection 기능을 이용해서 방향을 만들어서
		Direction = controlTransform.TransformVector(Direction);
		// 3. 그 방향으로 이동한다.
		AddMovementInput(Direction, 1);
		Direction = FVector::ZeroVector;
	}

	if (bRotatingToTarget)
	{
		RotateToTarget(TargetPrisoner);
	}
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
}

void APlayerCharacter::OnActionMove(const FInputActionValue& Value)
{
	if (IsLockedMove())
	{
		return;
	}

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
	if(bMoveInputPressed == false || IsLockedMove()) return;

	float currtime = GetWorld()->GetTimeSeconds();

	if (currtime - LastDodgeInputPressTime <= DoublePressInterval)
	{
		//GetCharacterMovement()->Velocity = GetActorUpVector() * 200;
        //Jump();

		PlayAnimMontage(DodgeMontage);

		// 구르기 시 공격 콤보 초기화
		SetAttackComboCount(0);	
	}
	LastDodgeInputPressTime = currtime;
}

/// <summary>
/// 기본 공격 함수
/// </summary>
/// <param name="Value"></param>
void APlayerCharacter::OnActionAttack(const FInputActionValue& Value)
{
	if (IsLockedMove())
	{
		//UE_LOG(LogTemp, Warning, TEXT("이미 이동 중이여서 공격 대상으로 이동할 수 없습니다."));
		return;
	}

	//if (PlayerAnim->bIgnoreInputAttack)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Attack을 할 수 없는 상태입니다."));
	//	return;
	//}

	bIsSlow = false;

	TargetPrisoner = FindTargetPrisoner();

	// 공격할 대상이 있다면
	if (TargetPrisoner != nullptr)
	{
		// 대상 위치로 이동
		bMovingToTarget = true;
		// 최대 스피드 증가
		GetCharacterMovement()->MaxWalkSpeed = AttackMaxSpeed;
		// 반격 상태라면 슬로우
		bIsSlow = TargetPrisoner->fsm->IsAttack();
	}
	// 공격할 수 있는 대상이 없다면, 앞방향으로 일정거리만큼 이동
	else
	{
		// 콤보 카운트 증가
		FString section = FString::FromInt((AnimComboCount % 3));
		// 애니메이션 실행
		PlayAnimMontage(FrontAttackMontage, 1, FName(section));
		AnimComboCount++;
		
		//PlayAttackAnimation();
		GetCharacterMovement()->Velocity = GetActorForwardVector() * 2000;
	}
}

void APlayerCharacter::OnActionBossAttack(const FInputActionValue& Value)
{
	if (TargetBoss == nullptr) return;
	if(IsLockedMove()) return;
    //if (AttackComboCount < MaxBossAttackComboCount) return;

	// 타켓의 위치에서 150 앞에 있는 위치
	// 이동할 위치 설정
	FVector offset = UKismetMathLibrary::GetDirectionUnitVector(TargetBoss->GetActorLocation(), GetActorLocation()) * 150;
	FVector targetLoc = TargetBoss->GetActorLocation() + offset;
	// 회전 설정
	FVector targetDir = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetBoss->GetActorLocation());
	FRotator targetRot = UKismetMathLibrary::MakeRotFromX(targetDir);
	
	//FVector offset = TargetBoss->GetActorRightVector() * 150;

	//FVector targetLoc = TargetBoss->GetActorLocation() + offset;
	//// 회전 설정
	//FVector targetDir = UKismetMathLibrary::GetDirectionUnitVector(offset, TargetBoss->GetActorLocation());
	//FRotator targetRot = UKismetMathLibrary::MakeRotFromX(targetDir);
	
	
	//// 모션 워핑 실행
	//MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("AttactPoint"), targetLoc, targetRot);

	//PlayerMotionWarpingComp->AddAndUpdateMotionWarping(EAttackType::DropKick);

	// 몽타주 재생
	PlayAnimMontage(BossAttackMotages[bossAttackIndex]);
	bossAttackIndex = (bossAttackIndex + 1) % BossAttackMotages.Num();

	// 시퀀스 재생
	MyGameModeBase->PlaySequence();
	
	// 공격 콤보 초기화
	SetAttackComboCount(0);

	PlayerAnim->SetIgnoreAttack(false);
}

void APlayerCharacter::MoveToTarget(AActor* Target)
{
	if (!Target) return;
	if (!bMovingToTarget) return;

	FVector dir = Target->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	AddMovementInput(dir.GetSafeNormal());

	PlayerAnim->SetRun(true);
	
	// 목표 지점에 도달했는지 확인
	if (dir.Size() < 100)
	{
		PlayerAnim->SetRun(false);

		bMovingToTarget = false;
		bRotatingToTarget = true;

		// 애니메이션 실행
		PlayAttackAnimation();
		// 최대 스피드 복구
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;

	}
}

bool APlayerCharacter::IsLockedMove() const
{
	bool bIsMontagePlaying = PlayerAnim->IsAnyMontagePlaying();

	return bMovingToTarget || bDamageState || PlayerAnim->bDodge || bIsMontagePlaying || MyGameModeBase->IsPlayingSequence();

}

void APlayerCharacter::RotateToTarget(AActor* Target)
{
	if (!Target) return;
	if (!bRotatingToTarget) return;

	// 회전값 갱신
	FVector targetLocation = Target->GetActorLocation();
	FVector direction = (targetLocation - GetActorLocation()).GetSafeNormal();
	FRotator targetRotation = FRotationMatrix::MakeFromX(direction).Rotator();

	FRotator newRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, GetWorld()->DeltaTimeSeconds, 8);
	SetActorRotation(newRotation);

	// 타겟과 마주보고 있다면 회전 종료
	FVector forwardVector = GetActorForwardVector();
	FVector targetForwardVector = Target->GetActorForwardVector();

	if (FVector::DotProduct(forwardVector, targetForwardVector) < -0.8f)
	{
		bRotatingToTarget = false;
	}
}

EEnemyDirection APlayerCharacter::GetTargetVerticalDirection(AActor* Target)
{
	// 적이 앞에 있는지, 뒤에 있는지 판별
	FVector targetVector = Target->GetActorLocation() - GetActorLocation();
	FVector forwardVector = GetActorForwardVector();

	float forwardDotProduct = FVector::DotProduct(targetVector, forwardVector);

	if(forwardDotProduct > 0)
		return EEnemyDirection::Front;
	else
		return EEnemyDirection::Back;
}

EEnemyDirection APlayerCharacter::GetTargetHorizontalDirection(AActor* Target)
{
	// 적이 왼쪽에 있는지, 오른쪽에 있는지 판별
	FVector targetVector = Target->GetActorLocation() - GetActorLocation();
	FVector rightVector = GetActorRightVector();

	float rightDotProject = FVector::DotProduct(targetVector, rightVector);

	if (rightDotProject > 0)
		return EEnemyDirection::Right;
	else
		return EEnemyDirection::Left;
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
	EEnemyDirection enemyDir = GetTargetVerticalDirection(TargetPrisoner);

	// 적이 앞에 있다면 콤보 애니메이션 실행
	if (enemyDir == EEnemyDirection::Front)
	{
		// 콤보 카운트 증가
		FString section = FString::FromInt((AnimComboCount % 3));
		// 애니메이션 실행
		PlayAnimMontage(FrontAttackMontage, 1, FName(section));
	}
	// 적이 뒤에 있다면 왼쪽, 오른쪽 구분해서 애니메이션 실행
	else
	{
		FString dirName = GetTargetHorizontalDirection(TargetPrisoner) == EEnemyDirection::Left ? "Left" : "Right";
		PlayAnimMontage(BackAttackMontage, 1, FName(dirName));
	}

	AnimComboCount++;
}

void APlayerCharacter::OnHitPrisoner()
{
	if(TargetPrisoner == nullptr) return;

	// hit 대상이 거리 내에 있다면 데미지 입히기
	float distance = FVector::Distance(TargetPrisoner->GetActorLocation(), GetActorLocation());
	if (distance < 150)
	{
		TargetPrisoner->fsm->OnMyTakeDamage(1);
	}
}

void APlayerCharacter::OnHitBoss()
{
	if (TargetBoss == nullptr) return;

	TargetBoss->fsm->OnMyTakeDamage(5);
}

void APlayerCharacter::OnPlayMotionWarping(EAttackType AttackType)
{
	PlayerMotionWarpingComp->AddAndUpdateMotionWarping(AttackType);
}

void APlayerCharacter::OnTakeDamage(AActor* OtherActor, int32 Damage)
{
	if(HP <= 0) return;
	if(bDamageState) return;
	if(MyGameModeBase->IsPlayingSequence()) return;

	HP -= Damage;
	
	// 공격 콤보 초기화
	SetAttackComboCount(0);

	// 슬로우 해제
	bIsSlow = false;

	// Damage 처리
	if (HP > 0)
	{
		bDamageState = true;

		// 맞은 방향으로 밀리기
		FVector dir = GetActorLocation() - OtherActor->GetActorLocation();
		dir.Z = 0;
		dir.Normalize();
		GetCharacterMovement()->Velocity = dir * 2000;

		// 적이 앞에서 공격했다면, 뒤로 휘청거리기
		EEnemyDirection dirState = GetTargetVerticalDirection(OtherActor);
		if (dirState == EEnemyDirection::Front)
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

		UE_LOG(LogTemp, Warning, TEXT("Player Damage!! : Hp = %d"), HP);
	}
	// Die 처리
	else
	{
		bDamageState = true;
		PlayerAnim->bDie = true;

		SetMeshCollisionEnabled(false);
		UE_LOG(LogTemp, Warning, TEXT("Player Die!!"), HP);
	}
}

void APlayerCharacter::OnEndDamage()
{
	bDamageState = false;
	PlayerAnim->bIgnoreInputAttack = false;
}

void APlayerCharacter::SetAttackComboCount(float Value)
{
	AttackComboCount = Value;

	UE_LOG(LogTemp, Warning, TEXT("AttackComboCount : %d"), AttackComboCount);
}

void APlayerCharacter::CallDelegateLevelSequnce()
{
}

void APlayerCharacter::ResetCombo()
{
	SetMeshCollisionEnabled(false);
	AnimComboCount = 0;
}

void APlayerCharacter::SetMeshCollisionEnabled(bool bValue)
{
	//if (bValue)
	//{
	//	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//}
	//else
	//{
	//	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}
}

void APlayerCharacter::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//auto* prisoner = Cast<APrisoner>(OtherActor);
	//if (prisoner != nullptr && TargetPrisoner == prisoner)
	//{
	//	auto* prisonerFSM = prisoner->GetComponentByClass<UPrisonerFSM>();
	//	if (prisonerFSM != nullptr)
	//	{
	//		bIsSlow = prisonerFSM->IsAttack();

	//		// 공격 콤보 증가
	//		SetAttackComboCount(AttackComboCount + 1);

	//		OverlapPrisoner = prisoner;
	//	}
	//}
	//if (MyGameModeBase->IsPlayingSequence())
	//{
 //       auto* boss = Cast<ABoss>(OtherActor);
 //       if (boss != nullptr)
 //       {
	//		UE_LOG(LogTemp, Warning, TEXT("Player->Boss Attack!!"));
 //           boss->fsm->OnMyTakeDamage(5);
 //       }
	//}

	//SetMeshCollisionEnabled(false);
}