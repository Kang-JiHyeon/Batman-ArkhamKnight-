// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMapMainWidget.h"
#include "PlayerStatWidget.h"

void UBossMapMainWidget::SetPlayerHPBar(const int32 Value, const int32 Max)
{
    PlayerStatWidget->SetHPBar(Value, Max);
}

void UBossMapMainWidget::SetPlayerCombo(const int32 Value, const int32 Max)
{
    PlayerStatWidget->SetComboGaze(Value, Max);
}

void UBossMapMainWidget::SetVisibilityPlayerHP(bool bValue)
{
}

void UBossMapMainWidget::SetVisibilityPlayerStat(bool bValue)
{
}

