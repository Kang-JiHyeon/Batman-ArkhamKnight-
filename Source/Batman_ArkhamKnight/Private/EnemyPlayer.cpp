// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AEnemyPlayer::AEnemyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 70, 90));
	SpringArmComp->TargetArmLength = 400;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void AEnemyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{
		// 현재 컨트롤러의 회전 방향을 가져옵니다.
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		// 이동 벡터를 회전 방향에 맞게 변환합니다.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * v + FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * h;
		SetActorLocation(GetActorLocation() + Direction * speed * DeltaTime);
	}
	
}

// Called to bind functionality to input
void AEnemyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &AEnemyPlayer::OnMyAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AEnemyPlayer::OnMyAxisVertical);

	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &AEnemyPlayer::OnMyLookHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &AEnemyPlayer::OnMyLookVertical);

}

void AEnemyPlayer::OnMyAxisHorizontal(float value)
{
	h = value;
}

void AEnemyPlayer::OnMyAxisVertical(float value)
{
	v = value;
}

void AEnemyPlayer::OnMyLookHorizontal(float Value)
{
	AddControllerYawInput(Value * MouseSensitivity);
}

void AEnemyPlayer::OnMyLookVertical(float Value)
{
	AddControllerPitchInput(Value * MouseSensitivity);
}



