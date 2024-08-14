// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossMapMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UBossMapMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UPlayerStatWidget* PlayerStatWidget;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UPlayerGameOverWidget* PlayerGameOverWidget;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UBossHP* BossHPWidget;

	UPROPERTY(EditDefaultsOnly,meta=(BindWidget))
	class UBossGameOverWidget* GameOverWidget ;

public:
	void UpdatePlayerHPBar(const int32 Value, const int32 Max);
	void UpdatePlayerHitCombo(const int32 Value, const int32 Max);
	void UpdatePlayerSkillGauge(const int32 Value, const int32 Max);

	void ShowGameOverWidget(bool bValue);

	void UpdateBossHPBar(int32 cur, int32 max);

	void VisibleOverUI();
	
	UPROPERTY(BlueprintReadOnly,Transient,meta = (BindWidgetAnim))
	class UWidgetAnimation* Red;

	UFUNCTION()
	void BlinkRedUI();
	void SetRedUI();
};
