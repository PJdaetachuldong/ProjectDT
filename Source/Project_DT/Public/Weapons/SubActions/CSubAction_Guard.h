// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStuctures.h"
#include "CSubAction_Guard.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class PROJECT_DT_API UCSubAction_Guard : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY ( EditAnywhere , Category = "Guard" )
	TArray<FDoActionData> ActionData;

	UPROPERTY ( EditAnywhere , Category = "Guard" )
	TArray<FHitData> HitDatas;
public:
	void Pressed ( ) override;
	void Released ( ) override;

	void Begin_SubAction_Implementation ( ) override;
	void End_SubAction_Implementation ( ) override;

private:
	TArray<class ACharacter*>Hitted;
	int32 HitIndex;

};
