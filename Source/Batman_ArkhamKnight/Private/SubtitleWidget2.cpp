// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget2.h"
#include "TimerManager.h"

void USubtitleWidget2::Show()
{
	if (Play2) {
		this->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USubtitleWidget2::Hide, 2.0f, false);
	}
}

void USubtitleWidget2::Hide()
{
	if (Play2) this->SetVisibility(ESlateVisibility::Hidden);
	Play2 = false;
}
