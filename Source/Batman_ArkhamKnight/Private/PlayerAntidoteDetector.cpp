// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAntidoteDetector.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerGameModeBase.h"

// Sets default values for this component's properties
UPlayerAntidoteDetector::UPlayerAntidoteDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPlayerAntidoteDetector::BeginPlay()
{
	Super::BeginPlay();

	MyGameModeBase = Cast<APlayerGameModeBase>(GetWorld()->GetAuthGameMode());
}

void UPlayerAntidoteDetector::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(IA_Interaction, ETriggerEvent::Triggered, this, &UPlayerAntidoteDetector::OnActionInteraction);
}

void UPlayerAntidoteDetector::OnActionInteraction()
{
	if (Target == nullptr)	return;
	if (MyGameModeBase == nullptr)	return;

	MyGameModeBase->VisibleUI();
}

void UPlayerAntidoteDetector::SetTargetAntidote(AActor* TargetAntidote)
{
	Target = TargetAntidote;
}
