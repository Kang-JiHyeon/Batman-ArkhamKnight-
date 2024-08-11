// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerStatWidget::SetHPBar(const int32 Value, const int32 Max)
{
    float percent = (float)Value / Max;
    HPBar->SetPercent(percent);
}

void UPlayerStatWidget::SetComboGaze(const int32 Value, const int32 Max)
{
    float percent = (float)Value / Max;

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
}


void UPlayerStatWidget::SetSkillGazeBar(const int32 Value, const int32 Max)
{
    float percent = (float)Value / Max;

    SkillGazeBar->SetPercent(percent);
}
