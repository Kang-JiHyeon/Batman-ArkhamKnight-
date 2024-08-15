// Fill out your copyright notice in the Description page of Project Settings.


#include "BossGameOverWidget.h"
#include "Components/CanvasPanel.h"
#include "GameFramework/PlayerController.h"

void UBossGameOverWidget::OnMyGameOverUI(bool visible)
{
    if (visible)
    {
        auto* pc = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
        pc->SetShowMouseCursor(true);
        pc->SetInputMode(FInputModeGameOnly());
        this->SetVisibility(ESlateVisibility::Visible);
        UE_LOG(LogTemp, Warning, TEXT("OnMyGameOverUI Visible!"));
    }
}
