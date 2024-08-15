// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget5.h"
#include "TimerManager.h"

void USubtitleWidget5::Show()
{
	if (Play5) {
		this->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USubtitleWidget5::Hide, 2.6f, false);
	}
}

void USubtitleWidget5::Hide()
{
	if (Play5) this->SetVisibility(ESlateVisibility::Hidden);
	Play5 = false;
}
