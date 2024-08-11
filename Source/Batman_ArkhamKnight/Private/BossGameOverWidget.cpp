// Fill out your copyright notice in the Description page of Project Settings.


#include "BossGameOverWidget.h"
#include "Components/CanvasPanel.h"

void UBossGameOverWidget::OnMyGameOverUI(bool visible)
{
    if (visible)
    {
        if (GameOver)
        {
            GameOver->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("OnMyGameOverUI Visible!"));
        }
    }

}
