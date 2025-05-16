// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CMeleeAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API ACMeleeAIController : public AAIController
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;
};
