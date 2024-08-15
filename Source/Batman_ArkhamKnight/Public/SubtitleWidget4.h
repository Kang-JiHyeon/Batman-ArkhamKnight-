// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubtitleWidget4.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API USubtitleWidget4 : public UUserWidget
{
	GENERATED_BODY()
	FTimerHandle TimerHandle;
public:
   UPROPERTY(meta = (BindWidget))
   class UCanvasPanel* Subtitle;

   void Show();
   void Hide();

   bool Play4 = true;
};
