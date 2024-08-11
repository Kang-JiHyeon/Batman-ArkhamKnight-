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

	/*���� ����*/
	class APlayerCharacter* Player;

	// �̵�

	// ������
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DodgeSound;

	// �Ϲ� ���� (3���� �� ����)
	UPROPERTY(EditDefaultsOnly)
	TArray<USoundBase*> ValidAttackSounds;
	int32 ValidAttackIdx;

	// �Ϲ� ����(����+��ȿŸ)
	UPROPERTY(EditDefaultsOnly)
	TArray<USoundBase*> InvalidAttackSounds;
	int32 InvalidAttackIdx;

	// ���� Ưȭ ���� (2���� �� ����)
	UPROPERTY(EditDefaultsOnly)
	TArray<USoundBase*> BossAttackSounds;
	int32 BossAttackIdx;

	// �ǰ�
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DamageSound;

	// ����
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
