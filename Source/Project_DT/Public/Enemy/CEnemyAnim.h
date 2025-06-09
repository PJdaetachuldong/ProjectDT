// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FSM/CMeleeEnemyFSM.h"
#include "CEnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
	public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;;

	class ACMeleeEnemy* Owner;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
// 	float Speed = 0.0f;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
// 	float MoveDirection = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
	EMeleeEnemyState State = EMeleeEnemyState::IDLE;
};
