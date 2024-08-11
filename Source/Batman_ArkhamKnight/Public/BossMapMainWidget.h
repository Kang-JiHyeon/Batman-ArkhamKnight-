// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossMapMainWidget.generated.h"

UCLASS()
class BATMAN_ARKHAMKNIGHT_API UBossMapMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UPlayerStatWidget* PlayerStatWidget;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UPlayerGameOverWidget* PlayerGameOverWidget;


public:
	void UpdatePlayerHPBar(const int32 Value, const int32 Max);
	void UpdatePlayerHitCombo(const int32 Value, const int32 Max);
	void UpdatePlayerSkillGauge(const int32 Value, const int32 Max);

	void ShowGameOverWidget(bool bValue);
};
