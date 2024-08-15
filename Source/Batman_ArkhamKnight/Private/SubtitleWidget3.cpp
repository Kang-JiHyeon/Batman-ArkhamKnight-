// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitleWidget3.h"
#include "TimerManager.h"

void USubtitleWidget3::Show()
{
	if (Play3) {
		this->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USubtitleWidget3::Hide, 2.6f, false);
	}
}

void USubtitleWidget3::Hide()
{
	if (Play3) this->SetVisibility(ESlateVisibility::Hidden);
	Play3 = false;
}
