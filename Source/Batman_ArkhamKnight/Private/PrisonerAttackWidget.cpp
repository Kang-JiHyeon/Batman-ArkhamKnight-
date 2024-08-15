// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerAttackWidget.h"
#include "Components/Image.h"

void UPrisonerAttackWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTIme)
{
	Super::NativeTick(MyGeometry, InDeltaTIme);
}

void UPrisonerAttackWidget::VisibleAttackUI()
{
	PrisonerAttackImage->SetVisibility(ESlateVisibility::Visible);

}
