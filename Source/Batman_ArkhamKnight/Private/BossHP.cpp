// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHP.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"

void UBossHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



void UBossHP::HideBossHP()
    {
        BossHPCanvas->SetVisibility(ESlateVisibility::Hidden);
    }

void UBossHP::HideRedBossHP()
{
    BossHpDelayBar->SetPercent((float)CurrentHP / MaxHP);
}

void UBossHP::SetBossHPBar(int32 cur, int32 max)
{

    CurrentHP = cur;
    MaxHP = max;

    BossHPBar->SetPercent((float)CurrentHP / MaxHP);

    if (CurrentHP == 0) // hp가 0일때 UI 삭제
    {
        GetWorld()->GetTimerManager().SetTimer(HPTimerHandle1, FTimerDelegate::CreateUObject(this, &UBossHP::HideBossHP), 2.0f, false);
    }
    else
    {
        GetWorld()->GetTimerManager().SetTimer(HPTimerHandle2, FTimerDelegate::CreateUObject(this, &UBossHP::HideRedBossHP), 2.0f, false);
    }
}


