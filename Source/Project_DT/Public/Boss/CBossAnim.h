// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FSM/CBossFSM.h"
#include "CBossAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCBossAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
	EBossState State = EBossState::IDLE;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = AttackState )
	EBossATTACKState AttackState = EBossATTACKState::NONE;
};
