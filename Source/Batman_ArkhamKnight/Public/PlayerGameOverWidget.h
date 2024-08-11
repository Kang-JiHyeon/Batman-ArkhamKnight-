// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATMAN_ARKHAMKNIGHT_API UPlayerGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* RestartBtn;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* QuitBtn;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULevel> RestartLevel;

public:
	UFUNCTION()
	void OnClickButtonRestart();
	UFUNCTION()
	void OnClickButtonQuit();

	void ShowWidget(bool bValue);
};
