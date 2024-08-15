// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget4.h"
#include "TimerManager.h"

void USubtitleWidget4::Show()
{
	if (Play4) {
		this->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USubtitleWidget4::Hide, 2.6f, false);
	}
}

void USubtitleWidget4::Hide()
{
	if (Play4) this->SetVisibility(ESlateVisibility::Hidden);
	Play4 = false;
}