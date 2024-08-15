// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget1.h"
#include "TimerManager.h"

void USubtitleWidget1::Show()
{
	if (Play1) {
		this->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USubtitleWidget1::Hide, 2.6f, false);
	}
}

void USubtitleWidget1::Hide()
{
	if (Play1) this->SetVisibility(ESlateVisibility::Hidden);
	Play1 = false;
}
