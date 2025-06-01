// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_TimeManager.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class PROJECT_DT_API UCAnimNotifyState_TimeManager : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Time")
float DelayTime=0.5f;
};
