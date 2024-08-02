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
#include "Prisoner.h"
#include "PrisonerFSM.h"
#include "TimerManager.h"


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
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMovingToTarget)
	{
		MoveToTarget(TargetEnemy);
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
		RotateToTarget(TargetEnemy);
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
	if(bMoveInputPressed == false || bMovingToTarget) return;

	float currtime = GetWorld()->GetTimeSeconds();

	if (currtime - LastDodgeInputPressTime <= DoublePressInterval)
	{
        if (PlayerAnim != nullptr)
        {
            PlayerAnim->SetDodge(true);
        }

		GetCharacterMovement()->Velocity = GetActorForwardVector() * DodgeSpeed;
        Jump();
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
		UE_LOG(LogTemp, Warning, TEXT("이미 이동 중이여서 공격 대상으로 이동할 수 없습니다."));
		return;
	}

	if (PlayerAnim->bIgnoreInputAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack을 할 수 없는 상태입니다."));
		return;
	}

	TArray<AActor*> targetActors;
	float minDistance = AttackRange;

	// Enemy를 모두 탐색
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PrisonerFactory, targetActors);

	// 탐색된 Enemy가 없을 경우 종료
	if (targetActors.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("탐색된 적이 없습니다."));
		return;
	}

	// 기존의 이동 대상을 초기화
	TargetEnemy = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("최단 거리의 적 탐색 중.."));
	for (AActor* targetActor : targetActors)
	{
		APrisoner* enemy = Cast<APrisoner>(targetActor);

		// TODO : 적이 무력화 상태라면, 다음으로 최단 거리에 있는 적을 향해 이동하고 싶다.
		//if (enemy == nullptr || enemy->GetValided())
		if (enemy == nullptr || enemy->IsAttackable() == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("적이 기절 상태입니다. 다음 적을 탐색합니다."));
			continue;
		}

		// Enemy와 나의 위치의 거리가 현재 탐색된 적과의 최소 거리보다 작다면, Target 대상 변경
		float distance = FVector::Distance(enemy->GetActorLocation(), GetActorLocation());
		if (distance < minDistance)
		{
			TargetEnemy = enemy;
			minDistance = distance;
		}
	}

	// 공격할 대상이 있다면
	if (TargetEnemy != nullptr)
	{
		// 대상 위치로 이동
		bMovingToTarget = true;

		// 최대 스피드 증가
		GetCharacterMovement()->MaxWalkSpeed = AttackMaxSpeed;

		// 매쉬 콜리전 활성화
		SetMeshCollisionEnabled(true);
	}
	// 공격할 수 있는 대상이 없다면, 앞방향으로 일정거리만큼 이동
	else
	{
		OnPlayAttackAnimation();
		GetCharacterMovement()->Velocity = GetActorForwardVector() * 2000;
	}
}

void APlayerCharacter::MoveToTarget(AActor* Target)
{
	if (!Target) return;
	if (!bMovingToTarget) return;

	FVector dir = Target->GetActorLocation() - GetActorLocation();
	dir.Z = 0;
	AddMovementInput(dir.GetSafeNormal());
	
	// 목표 지점에 도달했는지 확인
	if (dir.Size() < 100)
	{
		bMovingToTarget = false;

		bRotatingToTarget = true;
		// 애니메이션 실행
		OnPlayAttackAnimation();

		// 최대 스피드 복구
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
	}
}

bool APlayerCharacter::IsLockedMove() const
{
	return bMovingToTarget || PlayerAnim->bDodge || bDamageState;
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

EEnemyDirection APlayerCharacter::GetTargetVerticalDirection(AActor* TargetActor)
{
	// 적이 앞에 있는지, 뒤에 있는지 판별
	FVector targetVector = TargetActor->GetActorLocation() - GetActorLocation();
	FVector forwardVector = GetActorForwardVector();

	float forwardDotProduct = FVector::DotProduct(targetVector, forwardVector);

	if(forwardDotProduct > 0)
		return EEnemyDirection::Front;
	else
		return EEnemyDirection::Back;
}

EEnemyDirection APlayerCharacter::GetTargetHorizontalDirection(AActor* TargetActor)
{
	// 적이 왼쪽에 있는지, 오른쪽에 있는지 판별
	FVector targetVector = TargetActor->GetActorLocation() - GetActorLocation();
	FVector rightVector = GetActorRightVector();

	float rightDotProject = FVector::DotProduct(targetVector, rightVector);

	if (rightDotProject > 0)
		return EEnemyDirection::Right;
	else
		return EEnemyDirection::Left;
}


void APlayerCharacter::OnPlayAttackAnimation()
{
	if(TargetEnemy == nullptr) return;

	// 앞, 뒤 방향 확인
	EEnemyDirection enemyDir = GetTargetVerticalDirection(TargetEnemy);

	// 적이 앞에 있다면 콤보 애니메이션 실행
	if (enemyDir == EEnemyDirection::Front)
	{
		// 콤보 카운트 증가
		FString section = FString::FromInt((ComboCount % 3));
		// 애니메이션 실행
		PlayAnimMontage(FrontAttackMontage, 1, FName(section));
	}
	// 적이 뒤에 있다면 왼쪽, 오른쪽 구분해서 애니메이션 실행
	else
	{
		FString dirName = GetTargetHorizontalDirection(TargetEnemy) == EEnemyDirection::Left ? "Left" : "Right";
		
		PlayAnimMontage(BackAttackMontage, 1, FName(dirName));
	}
	ComboCount++;
}

void APlayerCharacter::OnDamageProcess(AActor* OtherActor, int32 Damage)
{
	if(HP <= 0) return;
	if(bDamageState) return;

	HP -= Damage;
	
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

		//PlayAnimMontage(DamageMontage, 1, FName("Die"));
		SetMeshCollisionEnabled(false);
		UE_LOG(LogTemp, Warning, TEXT("Player Die!!"), HP);
	}
}

void APlayerCharacter::OnEndDamage()
{
	bDamageState = false;
	PlayerAnim->bIgnoreInputAttack = false;
}

void APlayerCharacter::ResetCombo()
{
	SetMeshCollisionEnabled(false);
	ComboCount = 0;
}

void APlayerCharacter::SetMeshCollisionEnabled(bool bValue)
{
	if (bValue)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//UE_LOG(LogTemp, Warning, TEXT("Collision Enable : QueryAndPhysics!"));
	}
	else
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//UE_LOG(LogTemp, Warning, TEXT("Collision Enable : NoCollision"));
	}
}

void APlayerCharacter::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* prisoner = Cast<APrisoner>(OtherActor);

	if (prisoner != nullptr)
	{
		auto* prisonerFSM = prisoner->GetComponentByClass<UPrisonerFSM>();
		if (prisonerFSM != nullptr)
		{
			prisonerFSM->OnMyTakeDamage(1);

			UE_LOG(LogTemp, Warning, TEXT("Player->Prisoner Attack!!"));
		}
	}
	SetMeshCollisionEnabled(false);
}