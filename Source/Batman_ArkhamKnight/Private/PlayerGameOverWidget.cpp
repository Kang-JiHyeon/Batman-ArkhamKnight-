// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "kismet/KismetSystemLibrary.h"

void UPlayerGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    RestartBtn->OnClicked.AddDynamic(this, &UPlayerGameOverWidget::OnClickButtonRestart);
    QuitBtn->OnClicked.AddDynamic(this, &UPlayerGameOverWidget::OnClickButtonQuit);
}

void UPlayerGameOverWidget::OnClickButtonRestart()
{
    // 현재 레벨을 다시 열기
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));

    GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void UPlayerGameOverWidget::OnClickButtonQuit()
{
    // 게임 종료
    auto* PlayerController = GetWorld()->GetFirstPlayerController();
    UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}

void UPlayerGameOverWidget::ShowWidget(bool bValue)
{
    ESlateVisibility visibility = bValue ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
    this->SetVisibility(visibility);
}
