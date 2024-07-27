// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnim.h"
#include "Components/BoxComponent.h"

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
	//Input->BindAction(IA_Dodge, ETriggerEvent::Completed, this, &APlayerCharacter::OnActionDodgeCompleted);

}

void APlayerCharacter::OnActionMove(const FInputActionValue& Value)
{
	if (PlayerAnim->bDodge)
	{
		return;
	}

	FVector2D v = Value.Get<FVector2D>();

	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Normalize();


	if(bMove == false)
		bMove = true;
}

void APlayerCharacter::OnActionMoveCompleted(const FInputActionValue& Value)
{
	bMove = false;
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
	if(bMove == false) return;

	float currtime = GetWorld()->GetTimeSeconds();

	if (currtime - LastDodgeKeyPressTime <= DoublePressInterval)
	{
        if (PlayerAnim != nullptr)
        {
            PlayerAnim->SetDodge(true);
        }

		GetCharacterMovement()->Velocity = DodgeSpeed * GetActorForwardVector();
        Jump();
	}
	
	LastDodgeKeyPressTime = currtime;
}

void APlayerCharacter::OnActionAttack(const FInputActionValue& Value)
{
	
}
