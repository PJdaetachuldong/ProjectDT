// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/ICharacter.h"
#include "Component/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class PROJECT_DT_API ACEnemy : public ACharacter,public IICharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

private:
	UPROPERTY(EditAnywhere, Category = "Color")
		FLinearColor OriginColor = FLinearColor::White;

protected:
	UPROPERTY(VisibleAnywhere)
		class UCMointageComponent* Montages;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	//UPROPERTY(VisibleAnywhere)
		//class UCStatusComponent* Status;


protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	float TakeDamage ( float DamageAmount , struct FDamageEvent const& DamageEvent , class AController* EventInstigator , AActor* DamageCauser ) override;

protected:
	virtual void Hitted ( );

private:
	UFUNCTION()
	void RestoreColor();


private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} Damage;
private:
	FTimerHandle RestoreColor_TimerHandle;
};
