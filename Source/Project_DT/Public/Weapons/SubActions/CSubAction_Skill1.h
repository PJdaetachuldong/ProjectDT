// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/SubActions/CSubAction_Skill.h"
#include "../CWeaponStuctures.h"
#include "CSubAction_Skill1.generated.h"

/**
 *
 */
UCLASS( Blueprintable )
class PROJECT_DT_API UCSubAction_Skill1 : public UCSubAction_Skill
{
	GENERATED_BODY()

private:
	UPROPERTY ( EditAnywhere , Category = "Skill" )
	FDoActionData ActionData;

	UPROPERTY ( EditAnywhere , Category = "Skill" )
	TArray<FHitData> HitDatas;
public:
	void Pressed ( ) override;

	void Begin_SubAction_Skill_Implementation ( ) override;
	void End_SubAction_Skill_Implementation ( ) override;

private:
	TArray<class ACharacter*>Hitted;
	int32 HitIndex;

};
