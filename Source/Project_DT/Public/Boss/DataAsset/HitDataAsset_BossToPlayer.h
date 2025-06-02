// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../CBossWeaponStructure.h"
#include "HitDataAsset_BossToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UHitDataAsset_BossToPlayer : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FEnemyToPlayerHit> HitDatas;
};
