// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "BaseWheeledVehiclePawn.generated.h"

/**
 *	Writer : Lee Dong Geun
 *	Last Modified : 2024-08-08
 */

class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
class UInputMappingContext;
class UInputAction;
class UCameraShakeBase;
class UChaosWheeledVehicleMovementComponent;
class AMissile;
class AMachineGunBullet;
class ACannonBall;
class UNiagaraSystem;
class UNiagaraComponent;
class UAudioComponent;

struct FTimerHandle;

UCLASS()
class BATMAN_ARKHAMKNIGHT_API ABaseWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

	/** Components */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BattleModeWeapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MachineGun;
	
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
	UInputAction * IA_MachineGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_BoostCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_ToggleCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_LockOn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction * IA_Missile;

	//* SFX */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* EngineSoundComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* BoostSoundComponent;

	/** Chaos Vehicle Movement Component */
	
	UPROPERTY()
	UChaosWheeledVehicleMovementComponent* ChaosVehicleMovementComponent;

	//* Missile Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Missile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMissile> MissileClass;

	//* Camera State*/

	bool bCameraState; // false => BackCamera, true => FrontCamera

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CrackCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> BoostCameraShake;

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
	
	//* Battle Mode Machine Gun Timer */
	FTimerHandle MachineGunTimerHandle;

	//* Machine Gun */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MachineGun", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMachineGunBullet> MachineGunBulletClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MachineGun", meta=(AllowPrivateAccess="true"))
	float MachineGunFireRate = 0.15f;

	//* Spawn Possess Character Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PossessCharacter", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACharacter> PossessCharacterClass;

public:
	ABaseWheeledVehiclePawn();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	virtual void Tick(float DeltaTime) override;

	void ThrottleStart(const FInputActionValue& Value);
	void ThrottleTrigger(const FInputActionValue& Value);
	void ThrottleComplete(const FInputActionValue& Value);
	
	void BrakeTrigger(const FInputActionValue& Value);
	void BrakeComplete(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	void SteeringTrigger(const FInputActionValue& Value);
	void SteeringComplete(const FInputActionValue& Value);

	void MouseLeftStart(const FInputActionValue& Value);		// * Boost */
	void MouseLeftTrigger(const FInputActionValue& Value);		// * Boost */
	void MouseLeftComplete(const FInputActionValue& Value);		// * Boost */
	void MouseRight(const FInputActionValue& Value);			// * Shot Mahcine Gun */ 
	void BoostStart(const FInputActionValue& Value);			// * Boost Start */
	void BoostEnd(const FInputActionValue& Value);				// * Boost End */
	UFUNCTION(BlueprintImplementableEvent)
	void BoostCameraLerp();										// * Boost Camera Lerp */
	
	void ToggleCamera();										// * Camera Switching */
	
	void LockOn(const FInputActionValue& Value);				// * Lock On */
	void Shot(const FInputActionValue& Value);					// * Execute FireMissile */	
	void FireMissile();											// * Shot Missile */

	void FireMachineGun();										// * Fire Machine Gun */

	UFUNCTION(BlueprintImplementableEvent)
	void PossessBatman();										// * Posses Batman */
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
