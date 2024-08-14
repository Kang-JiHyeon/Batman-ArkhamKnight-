// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttackWidget.h"
#include "Components/Image.h"

void UBossAttackWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTIme)
{
	Super::NativeTick(MyGeometry, InDeltaTIme);
}

void UBossAttackWidget::VisibleAttackUI()
{
	BossAttackImage->SetVisibility(ESlateVisibility::Visible);
}