// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LHW_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API ALHW_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	ALHW_GameModeBase();

	// 게임 시작 시 호출
	virtual void BeginPlay() override;

	// 매 프레임 호출
	virtual void Tick(float DeltaTime) override;
	
};
