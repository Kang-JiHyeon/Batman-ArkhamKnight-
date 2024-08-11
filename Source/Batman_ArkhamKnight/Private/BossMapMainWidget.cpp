// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMapMainWidget.h"
#include "PlayerStatWidget.h"

void UBossMapMainWidget::UpdatePlayerHPBar(const int32 Value, const int32 Max)
{
    PlayerStatWidget->SetHPBar(Value, Max);
}

void UBossMapMainWidget::UpdatePlayerHitCombo(const int32 Value, const int32 Max)
{
    PlayerStatWidget->SetAttackCombo(Value, Max);
}

void UBossMapMainWidget::UpdatePlayerSkillGauge(const int32 Value, const int32 Max)
{
    PlayerStatWidget->UpdateSkillGauge(Value, Max);
}

void UBossMapMainWidget::SetVisibilityPlayerHP(bool bValue)
{
}

void UBossMapMainWidget::SetVisibilityPlayerStat(bool bValue)
{
}

