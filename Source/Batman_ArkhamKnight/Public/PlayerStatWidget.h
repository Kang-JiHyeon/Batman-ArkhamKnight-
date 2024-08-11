// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ComboGazeBar;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ComboText;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* SkillGazeBar;

	void SetHPBar(const int32 Value, const int32 Max);
	void SetComboGaze(const int32 Value, const int32 Max);
	void SetSkillGazeBar(const int32 Value, const int32 Max);
};
