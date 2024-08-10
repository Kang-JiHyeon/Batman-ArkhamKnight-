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
	
public:
	UPROPERTY(meta=(BIndWidget))
	class UProgressBar* BossHPBar;
	
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	void SetBossHPBar(float cur, float max);

};
