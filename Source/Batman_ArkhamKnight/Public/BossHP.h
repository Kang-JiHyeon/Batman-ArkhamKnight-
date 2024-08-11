// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHP.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UBossHP : public UUserWidget
{
	GENERATED_BODY()
private:
	FTimerHandle HideBossHPTimerHandle;
public:
	UPROPERTY(meta=(BIndWidget))
	class UProgressBar* BossHPBar;

	UPROPERTY(meta = (BIndWidget))
	class UCanvasPanel* BossHP;
	
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	void SetBossHPBar(int32 cur, int32 max);

	void HideBossHP();
	float currentTime = 0;
};
