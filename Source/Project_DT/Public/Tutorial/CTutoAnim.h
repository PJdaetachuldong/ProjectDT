// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CTutorialEnemy.h"
#include "CTutoAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCTutoAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State )
	ETutoState AnimState = ETutoState::IDLE;
};
