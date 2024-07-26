// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "EnemyPlayer.generated.h"

UCLASS()
class BATMAN_ARKHAMKNIGHT_API AEnemyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,Category = Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* CameraComp;

	float h;
	float v;

	void OnMyAxisHorizontal(float value);
	void OnMyAxisVertical(float value);

	float speed = 400;

	void OnMyLookHorizontal(float Value);
	void OnMyLookVertical(float Value);

	float MouseSensitivity = 1.0f;

};
