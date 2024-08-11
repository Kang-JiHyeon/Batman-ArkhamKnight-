// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UBossGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BIndWidget))
	class UCanvasPanel* GameOver;

	void OnMyGameOverUI(bool visible);
};
