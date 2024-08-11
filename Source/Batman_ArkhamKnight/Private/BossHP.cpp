// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHP.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"

void UBossHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBossHP::SetBossHPBar(int32 cur, int32 max)
{
	BossHPBar->SetPercent((float)(cur) / max);
	if (cur <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(HideBossHPTimerHandle, this, &UBossHP::HideBossHP, 1.0f, false);
	}
}
void UBossHP::HideBossHP()
{
	BossHP->SetVisibility(ESlateVisibility::Hidden);
}
