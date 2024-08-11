// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSoundManager.generated.h"

UENUM()
enum class EPlayerSoundType
{
	Move,
	Dodge,
	VaildAttack,
	InvaildAttack,
	BossAttack,
	Damage,
	Die
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATMAN_ARKHAMKNIGHT_API UPlayerSoundManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSoundManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/*사운드 관리*/
	class APlayerCharacter* Player;

	// 이동

	// 구르기
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DodgeSound;

	// 일반 공격 (3가지 중 랜덤)
	UPROPERTY(EditDefaultsOnly)
	TArray<USoundBase*> ValidAttackSounds;
	int32 ValidAttackIdx;

	// 일반 공격(기합+무효타)
	UPROPERTY(EditDefaultsOnly)
	TArray<USoundBase*> InvalidAttackSounds;
	int32 InvalidAttackIdx;

	// 보스 특화 공격 (2가지 중 랜덤)
	UPROPERTY(EditDefaultsOnly)
	TArray<USoundBase*> BossAttackSounds;
	int32 BossAttackIdx;

	// 피격
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DamageSound;

	// 죽음
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DieSound;

	float StartTime;
	
public:
	void PlaySound(EPlayerSoundType SoundType);

private:
	void PlayDodge();
	void PlayValidAttack();
	void PlayInvalidAttack();
	void PlayBossAttack();
	void PlayDamage();
	void PlayDie();

	void SetSoundBase(USoundBase* SoundBase);
	int32 GetRandomIndex(int32 Min, int32 Max, int32 Curr);

};
