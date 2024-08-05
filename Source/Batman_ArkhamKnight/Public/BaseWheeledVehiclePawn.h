// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "BaseWheeledVehiclePawn.generated.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-08-04
 */

class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
class UInputMappingContext;
class UInputAction;
class UChaosWheeledVehicleMovementComponent;
class AMissile;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class BATMAN_ARKHAMKNIGHT_API ABaseWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

	/** Components */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BackCamera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* BackSpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BackCamera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* BackCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FrontCamera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* FrontSpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FrontCamera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FrontCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpawnLocation", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* MissileSpawnLocationLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpawnLocation", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* MissileSpawnLocationRight;

	TArray<UArrowComponent*> MissileSpawnLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* BoostVFXSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boost", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* LeftBoostVFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boost", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* RightBoostVFXComponent;

	/** Input Actions */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* BatMobileMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_Throttle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_Brake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_Steering;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_Boost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_ToggleCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_LockOn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_Missile;

	/** Chaos Vehicle Movement Component */
	
	UPROPERTY()
	UChaosWheeledVehicleMovementComponent* ChaosVehicleMovementComponent;

	//* Missile Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Missile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMissile> MissileClass;

	//* Camera State*/

	bool bCameraState; // false => BackCamera, true => FrontCamera

	//* Target Info*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* TargetActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BoostSpeed", meta = (AllowPrivateAccess = "true"))
	float BoostSpeed = 10000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BoostSpeed", meta = (AllowPrivateAccess = "true"))
	float BaseSpeed = 5000.f;
	
	FVector TargetLocation;
	float TargetDistance;
	bool bIsLockOn;

public:
	ABaseWheeledVehiclePawn();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	virtual void Tick(float DeltaTime) override;

	void ThrottleTrigger(const FInputActionValue& Value);
	void ThrottleComplete(const FInputActionValue& Value);
	void BrakeTrigger(const FInputActionValue& Value);
	void BrakeStart(const FInputActionValue& Value);
	void BrakeComplete(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SteeringTrigger(const FInputActionValue& Value);
	void SteeringComplete(const FInputActionValue& Value);
	void BoostTrigger(const FInputActionValue& Value);
	void BoostComplete(const FInputActionValue& Value);
	void ToggleCamera();
	void LockOn(const FInputActionValue& Value);
	void Shot(const FInputActionValue& Value);
	void FireMissile();
};