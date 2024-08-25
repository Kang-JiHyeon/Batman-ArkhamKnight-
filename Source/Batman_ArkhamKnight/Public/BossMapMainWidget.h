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

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USubtitleWidget1* BossSubtitleWidget1;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USubtitleWidget2* BossSubtitleWidget2;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USubtitleWidget3* BossSubtitleWidget3;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USubtitleWidget4* BossSubtitleWidget4;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USubtitleWidget5* BossSubtitleWidget5;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class USubtitleWidget6* BossSubtitleWidget6;

public:
	void UpdatePlayerHPBar(const int32 Value, const int32 Max);
	void UpdatePlayerHitCombo(const int32 Value, const int32 Max);
	void UpdatePlayerSkillGauge(const int32 Value, const int32 Max);

	void ShowGameOverWidget(bool bValue);
	void ShowPlayerStatWidget(bool bValue);

	void UpdateBossHPBar(int32 cur, int32 max);

	void VisibleOverUI();
	
	UPROPERTY(BlueprintReadOnly,Transient,meta = (BindWidgetAnim))
	class UWidgetAnimation* Red;

	UPROPERTY(BlueprintReadOnly,Transient,meta = (BindWidgetAnim))
	class UWidgetAnimation* RedAll;

	void BlinkRedUI();
	void BlinkRedAllUI();

	void ShowSubtitle1();
	void ShowSubtitle2();
	void ShowSubtitle3();
	void ShowSubtitle4();
	void ShowSubtitle5();
	void ShowSubtitle6();

};
