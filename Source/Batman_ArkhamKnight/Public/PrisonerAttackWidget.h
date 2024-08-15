// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PrisonerAttackWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UPrisonerAttackWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* PrisonerAttackImage;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTIme);
	void VisibleAttackUI();
};
