// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Prisoner.generated.h"

UCLASS()
class BATMAN_ARKHAMKNIGHT_API APrisoner : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APrisoner();
	// FSM에서 죽음 상태로 전환될 때 호출되는 함수
	void OnDeathStateEntered();

	FTimerHandle TimeHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,Category = FSMComponent)
	class UPrisonerFSM* fsm;

	UPROPERTY(EditAnywhere)
	class APrisoner* me;

	bool IsAttackable();
	
	// 띠링 생성해서 머리위에 배치
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UWidgetComponent* AttackComp;

	void Visible();
	void Hide();

};
