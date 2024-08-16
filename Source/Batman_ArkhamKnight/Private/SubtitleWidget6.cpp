// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget6.h"
#include "TimerManager.h"

void USubtitleWidget6::Show()
{
	if (Play6) {
		this->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USubtitleWidget6::Hide, 1.6f, false);
	}
}

void USubtitleWidget6::Hide()
{
	if (Play6) this->SetVisibility(ESlateVisibility::Hidden);
	Play6 = false;
}