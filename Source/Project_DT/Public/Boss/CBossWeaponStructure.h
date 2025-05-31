// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DamageEvents.h"
#include "CBossWeaponStructure.generated.h"

USTRUCT()
struct FEnemyToPlayerHit 
{
	GENERATED_BODY ( )
public:
	UPROPERTY ( EditAnywhere )
	class UAnimMontage* Montage;
	UPROPERTY ( EditAnywhere )
	float PlayRate = 1;
	UPROPERTY ( EditAnywhere )
	float Power;
	UPROPERTY ( EditAnywhere )
	float Launch = 100;
	UPROPERTY(EditAnywhere)
	float StopTime;
	UPROPERTY ( EditAnywhere )
	class USoundWave* Sound;
	UPROPERTY ( EditAnywhere )
	class UFXSystemAsset* Effect;
	UPROPERTY(EditAnywhere)
	FVector EffectLocation=FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

public:
	void SendDamage ( class ACharacter* InAttacker , AActor* InAttackCauser , class ACharacter* InOther );
	void PlayMontage ( class ACharacter* InOwner );
	void PlayHitStop ( UWorld* InWorld );
	void PlaySoundWave ( class ACharacter* InOwner );
	void PlayEffect ( UWorld* InWorld , const FVector& InLocation );
	void PlayEffect ( UWorld* InWorld , const FVector& InLocation , const FRotator& InRotation );
};

USTRUCT()
struct FEnemyDamageEvent :public FDamageEvent
{
	GENERATED_BODY()
public:
	FEnemyToPlayerHit* HitData;


};

UCLASS()
class PROJECT_DT_API UCBossWeaponStructure : public UObject
{
	GENERATED_BODY()
};
