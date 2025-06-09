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
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;;

	class ACBossEnemy* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bool)
	bool IsPlayingIdle = false;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
// 	float TargetYaw = 0.0f;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
// 	float DeltaYaw = 0.0f;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bool)
// 	bool IsTurnIDLE = false;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
// 	float TurnIdleDirection = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
	float MoveDirection = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
	EBossState State = EBossState::IDLE;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = AttackState )
	EBossATTACKState AttackState = EBossATTACKState::NONE;
};
