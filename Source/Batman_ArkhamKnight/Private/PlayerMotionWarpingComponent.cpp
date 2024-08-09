// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMotionWarpingComponent.h"
#include "PlayerCharacter.h"
#include "Boss.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../../../Plugins/Animation/MotionWarping/Source/MotionWarping/Public/MotionWarpingComponent.h"
//#include "PlayerAttackPointComponent.h"


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

}



// Called every frame
void UPlayerMotionWarpingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UPlayerMotionWarpingComponent::OnInitialize(AActor* OtherActor)
{
	if(OtherActor == nullptr) return;

	TargetActor = OtherActor;

	TArray<UActorComponent*> components = TargetActor->K2_GetComponentsByClass(UPlayerAttackPointComponent::StaticClass());

	if (components.Num() <= 0) return;

	for (auto component : components)
	{
		auto* attackPointComp = Cast<UPlayerAttackPointComponent>(component);

		if (attackPointComp != nullptr)
		{
			AttackPoints.Add(attackPointComp->AttackType, attackPointComp);
		}
	}
}

//void UPlayerMotionWarpingComponent::AddMotionWarping(EWarpingPoint TargetPoint)
//{
	//FVector targetDir = UKismetMathLibrary::GetDirectionUnitVector(Me->GetActorLocation(), Me->TargetBoss->GetActorLocation());
	//FRotator targetRot = UKismetMathLibrary::MakeRotFromX(targetDir);

	//const UEnum* CharStateEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWarpingPoint"), true);

	//FName EnumToName = FName(TEXT("Invalid"));
	//FString EnumToString = TEXT("Invalid");
	//if (CharStateEnum)
	//{
	//	EnumToName = CharStateEnum->GetNameByValue((int64)TargetPoint);
	//	EnumToString = CharStateEnum->GetNameStringByValue((int64)TargetPoint);
	//}


	//FVector targetLoc = Me->TargetBoss->GetActorLocation()+ WarpingInfos[TargetPoint].RelativeLocation;
	////FRotator targetRot = WarpingInfos[TargetPoint].RelativeRotation;

 //   //FName pointName = UEnum::GetValueAsName(TargetPoint);
	//UE_LOG(LogTemp, Warning, TEXT("AddMotionWarping : %s"), *EnumToString);

 //   Me->MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(EnumToString), targetLoc, targetRot);
    
//}
//
//void UPlayerMotionWarpingComponent::RemoveMotionWarping(EWarpingPoint TargetPoint)
//{
//}

