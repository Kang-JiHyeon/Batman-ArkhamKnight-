// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMapMainWidget.h"
#include "PlayerStatWidget.h"
#include "PlayerGameOverWidget.h"
#include "Kismet/GameplayStatics.h"

void UBossMapMainWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    ShowGameOverWidget(false);
}


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

void UBossMapMainWidget::ShowGameOverWidget(bool bValue)
{
    PlayerGameOverWidget->ShowWidget(bValue);

    GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(bValue);
    UGameplayStatics::SetGamePaused(GetWorld(), bValue);
}