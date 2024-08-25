// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMapMainWidget.h"
#include "PlayerStatWidget.h"
#include "PlayerGameOverWidget.h"
#include "BossHP.h"
#include "BossGameOverWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "SubtitleWidget1.h"
#include "SubtitleWidget2.h"
#include "SubtitleWidget3.h"
#include "SubtitleWidget4.h"
#include "SubtitleWidget5.h"
#include "SubtitleWidget6.h"

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
    ShowPlayerStatWidget(!bValue);
    GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(bValue);
    UGameplayStatics::SetGamePaused(GetWorld(), bValue);
}

void UBossMapMainWidget::ShowPlayerStatWidget(bool bValue)
{
    PlayerStatWidget->ShowWidget(bValue);
}

void UBossMapMainWidget::UpdateBossHPBar(int32 cur, int32 max)
{
    BossHPWidget->SetBossHPBar(cur, max);
}

void UBossMapMainWidget::VisibleOverUI()
{
    GameOverWidget->OnMyGameOverUI(true);

    GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
    
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UBossMapMainWidget::BlinkRedUI()
{
    PlayAnimation(Red);
}

void UBossMapMainWidget::BlinkRedAllUI()
{
    PlayAnimation(RedAll);
}

void UBossMapMainWidget::ShowSubtitle1()
{
    BossSubtitleWidget1->Show();
}
void UBossMapMainWidget::ShowSubtitle2()
{
    BossSubtitleWidget2->Show(); 
}
void UBossMapMainWidget::ShowSubtitle3()
{
    BossSubtitleWidget3->Show();
}
void UBossMapMainWidget::ShowSubtitle4()
{
    BossSubtitleWidget4->Show();
}
void UBossMapMainWidget::ShowSubtitle5()
{
    BossSubtitleWidget5->Show();
}
void UBossMapMainWidget::ShowSubtitle6()
{
    BossSubtitleWidget6->Show();
}
