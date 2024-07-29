// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "BaseWheeledVehiclePawn.generated.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-07-28
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API ABaseWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

	/** Components */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BackCamera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* BackSpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BackCamera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* BackCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FrontCamera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* FrontSpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FrontCamera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FrontCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpawnLocation", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* MissileSpawnLocationLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpawnLocation", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* MissileSpawnLocationUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MissileSpawnLocation", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* MissileSpawnLocationRight;

	/** Input Actions */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* BatMobileMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * IA_Throttle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * IA_Brake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * IA_Steering;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * IA_Boost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * IA_ToggleCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * IA_LockOn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * IA_Missile;

	/** Chaos Vehicle Movement Component */
	
	UPROPERTY()
	class UChaosWheeledVehicleMovementComponent* ChaosVehicleMovementComponent;

	//* Missile Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Missile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AMissile> MissileClass;

	//* Camera State*/

	bool bCameraState; // false => BackCamera, true => FrontCamera

	//* Target Info*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AActor* TargetActor;
	
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
