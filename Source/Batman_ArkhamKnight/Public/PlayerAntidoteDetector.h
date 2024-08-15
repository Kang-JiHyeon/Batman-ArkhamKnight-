// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAntidoteDetector.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UPlayerAntidoteDetector : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAntidoteDetector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

private:
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Interaction;

	UPROPERTY()
	class APlayerGameModeBase* MyGameModeBase;

	UPROPERTY()
	class AActor* Target;

private:
	void OnActionInteraction();

public:
	UFUNCTION(BlueprintCallable)
	void SetTargetAntidote(AActor* TargetAntidote);

};
