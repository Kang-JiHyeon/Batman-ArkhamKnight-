// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHP.h"
#include "Components/ProgressBar.h"

void UBossHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBossHP::SetBossHPBar(float cur, float max)
{
	BossHPBar->SetPercent(cur / max);
}

