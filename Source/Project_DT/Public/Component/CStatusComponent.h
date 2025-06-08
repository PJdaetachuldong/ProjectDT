// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHpSettingDelegate, float, HP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FManaSettingDelegate, float, Mana);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FHpSettingDelegate OnDelegateHP;
	FHpSettingDelegate OnDelegateMana;
private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere, Category = "Mana")
	float MaxMana = 100;



public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetMaxMana ( ) { return MaxMana; }

public:
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMana ( ) { return Mana; }
	FORCEINLINE bool IsDead() { return Health <= 0.0f; }

public:
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	float Damage(float InAmount);
	void Heal ( float InAmount );
	void UseMana ( float InAmount );
	void RecoverMana ( float InAmount );

private:
	void SetHP(float HP);
	void SetMana(float SetMana);

private:
	class ACharacter* OwnerCharacter;

private:
	float Health;
	float Mana;
};
