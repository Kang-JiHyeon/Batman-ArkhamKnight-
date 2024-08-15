// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

void UPlayerStatWidget::SetHPBar(const int32 Value, const int32 Max)
{
    float percent = (float)Value / Max;
    HPBar->SetPercent(percent);
}

void UPlayerStatWidget::SetAttackCombo(const int32 Value, const int32 Max)
{
    float percent = FMath::Clamp((float)Value / Max, 0, 1);

    ComboGazeBar->SetPercent(percent);

    if (Value <= 0)
    {
        ComboText->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        FText text = FText::Format(FText::FromString("x{0}"), FText::AsNumber(Value));
        ComboText->SetText(text);
        ComboText->SetVisibility(ESlateVisibility::Visible);
    }

    FLinearColor color = percent < 1 ? DefaultGaugeColor : ComboGaugeColor;
    ComboGazeBar->SetFillColorAndOpacity(color);
    ComboText->SetShadowColorAndOpacity(color);

}

void UPlayerStatWidget::UpdateSkillGauge(const int32 Value, const int32 Max)
{
    float persent = FMath::Clamp((float)Value / Max, 0, 1);

    if (persent >= 1)
    {
        SkillCanvasPanel->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        SkillCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
    }

    UpdateCirculerProgressBar(persent);
}

void UPlayerStatWidget::ShowWidget(bool bValue)
{
    ESlateVisibility visibility = bValue ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
    this->SetVisibility(visibility);
}
