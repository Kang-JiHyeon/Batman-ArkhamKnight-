// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMotionWarpingComponent.h"
#include "PlayerCharacter.h"
#include "Boss.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../../../Plugins/Animation/MotionWarping/Source/MotionWarping/Public/MotionWarpingComponent.h"

// Sets default values for this component's properties
UPlayerMotionWarpingComponent::UPlayerMotionWarpingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UPlayerMotionWarpingComponent::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<APlayerCharacter>(GetOwner());

	//FVector targetDir = UKismetMathLibrary::GetDirectionUnitVector(Me->GetActorLocation(), Me->TargetBoss->GetActorLocation());
	//FRotator targetRot = UKismetMathLibrary::MakeRotFromX(targetDir);

	//WarpingInfos[EWarpingPoint::Origin].RelativeLocation = FVector::ZeroVector;
	//WarpingInfos[EWarpingPoint::Origin].RelativeRotation = targetRot;

	//WarpingInfos[EWarpingPoint::Front].RelativeLocation = Me->TargetBoss->GetActorForwardVector() * 50;
	//WarpingInfos[EWarpingPoint::Front].RelativeRotation = targetRot;

	//WarpingInfos[EWarpingPoint::Right].RelativeLocation = Me->TargetBoss->GetActorRightVector() * 50;
	//WarpingInfos[EWarpingPoint::Origin].RelativeRotation = targetRot;
}



// Called every frame
void UPlayerMotionWarpingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UPlayerMotionWarpingComponent::OnInitialize()
{
	if(Me == nullptr || Me->TargetBoss == nullptr) return;

	FVector targetDir = UKismetMathLibrary::GetDirectionUnitVector(Me->GetActorLocation(), Me->TargetBoss->GetActorLocation());
	FRotator targetRot = UKismetMathLibrary::MakeRotFromX(targetDir);

	FWarpingTransform warpingTr;
	WarpingInfos.Add(EWarpingPoint::Origin, warpingTr);
	WarpingInfos.Add(EWarpingPoint::Front, warpingTr);
	WarpingInfos.Add(EWarpingPoint::Right, warpingTr);

	WarpingInfos[EWarpingPoint::Origin].RelativeLocation = FVector::ZeroVector;
	WarpingInfos[EWarpingPoint::Origin].RelativeRotation = targetRot;

	WarpingInfos[EWarpingPoint::Front].RelativeLocation = Me->TargetBoss->GetActorForwardVector() * 50;
	WarpingInfos[EWarpingPoint::Front].RelativeRotation = targetRot;

	WarpingInfos[EWarpingPoint::Right].RelativeLocation = Me->TargetBoss->GetActorRightVector() * 50;
	WarpingInfos[EWarpingPoint::Right].RelativeRotation = targetRot;
}

void UPlayerMotionWarpingComponent::AddMotionWarping(EWarpingPoint TargetPoint)
{
	FVector targetDir = UKismetMathLibrary::GetDirectionUnitVector(Me->GetActorLocation(), Me->TargetBoss->GetActorLocation());
	FRotator targetRot = UKismetMathLibrary::MakeRotFromX(targetDir);

	const UEnum* CharStateEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWarpingPoint"), true);

	FName EnumToName = FName(TEXT("Invalid"));
	FString EnumToString = TEXT("Invalid");
	if (CharStateEnum)
	{
		EnumToName = CharStateEnum->GetNameByValue((int64)TargetPoint);
		EnumToString = CharStateEnum->GetNameStringByValue((int64)TargetPoint);
	}


	FVector targetLoc = Me->TargetBoss->GetActorLocation()+ WarpingInfos[TargetPoint].RelativeLocation;
	//FRotator targetRot = WarpingInfos[TargetPoint].RelativeRotation;

    //FName pointName = UEnum::GetValueAsName(TargetPoint);
	UE_LOG(LogTemp, Warning, TEXT("AddMotionWarping : %s"), *EnumToString);

    Me->MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(EnumToString), targetLoc, targetRot);
    
}

void UPlayerMotionWarpingComponent::RemoveMotionWarping(EWarpingPoint TargetPoint)
{
}

