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
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMovingToTarget)
	{
		MoveToTarget(TargetEnemy);
		RotateToTarget(TargetEnemy);
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
	//Input->BindAction(IA_Dodge, ETriggerEvent::Completed, this, &APlayerCharacter::OnActionDodgeCompleted);

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

	TArray<AActor*> targetActors;
	float minDistance = AttackRange;

	// Enemy를 모두 탐색
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyFactory, targetActors);

	// 탐색된 Enemy가 없을 경우 종료
	if (targetActors.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("탐색된 적이 없습니다."));
		return;
	}

	//// 가장 가까이 있는 적이 이전 적이랑 같다면, 다음으로 가까운 적을 찾고 싶다.
	//if (targetActors.Contains(TargetEnemy))
	//{
	//	targetActors.Remove(TargetEnemy);
	//}

	// 기존의 이동 대상을 초기화
	TargetEnemy = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("최단 거리의 적 탐색 중.."));
	for (AActor* targetActor : targetActors)
	{
		ATestEnemy* enemy = Cast<ATestEnemy>(targetActor);

		// TODO : 적이 무력화 상태라면, 다음으로 최단 거리에 있는 적을 향해 이동하고 싶다.
		if (enemy == nullptr || enemy->GetValided())
		{
			UE_LOG(LogTemp, Warning, TEXT("적이 기절 상태입니다. 다음 적을 탐색합니다."));
			continue;
		}


		//  Enemy와 나의 위치의 거리가 현재 탐색된 적과의 최소 거리보다 작다면, Target 대상 변경
		float distance = FVector::Dist(enemy->GetActorLocation(), GetActorLocation());
		if (distance < minDistance)
		{
			TargetEnemy = enemy;
			minDistance = distance;

			UE_LOG(LogTemp, Warning, TEXT("최단 거리의 적 갱신!"));
		}
	}

	// 공격할 대상이 있다면, 그 위치로 이동
	if (TargetEnemy != nullptr)
	{
		bMovingToTarget = true;
		PlayerAnim->SetAttack(true);
	}
	// 공격할 수 있는 대상이 없다면, 앞방향으로 일정거리만큼 이동
	else
	{
		GetCharacterMovement()->Velocity = GetActorForwardVector() * 2000;
		UE_LOG(LogTemp, Warning, TEXT("적 발견 실패, 앞방향으로 이동합니다."));

	}
}

void APlayerCharacter::MoveToTarget(AActor* target)
{
	if (!target) return;
	if (!bMovingToTarget) return;

	FVector targetLocation = target->GetActorLocation();

	FVector newLocation = FMath::VInterpTo(GetActorLocation(), targetLocation, GetWorld()->DeltaTimeSeconds, 5.0f);
	SetActorLocation(newLocation);
	// 목표 지점에 도달했는지 확인
	if (FVector::Dist(GetActorLocation(), targetLocation) < 50)
	{
		bMovingToTarget = false;
		
	}
}

void APlayerCharacter::RotateToTarget(AActor* Target)
{
	if (!Target) return;
	//if (!bRotatingToTarget) return;

	FVector targetLocation = Target->GetActorLocation();
	FVector direction = (targetLocation - GetActorLocation()).GetSafeNormal();
	FRotator targetRotation = FRotationMatrix::MakeFromX(direction).Rotator();

	FRotator newRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, GetWorld()->DeltaTimeSeconds, 10);
	SetActorRotation(newRotation);
}

bool APlayerCharacter::IsLockedMove() const
{
	return bMovingToTarget || PlayerAnim->bDodge;
}
