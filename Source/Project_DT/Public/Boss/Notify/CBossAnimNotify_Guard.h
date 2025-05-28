// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CBossAnimNotify_Guard.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCBossAnimNotify_Guard : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;

	float CurTime = 0.0f;
	float DelayTime = 0.3f;

	virtual void NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration ) override;

	virtual void NotifyTick ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float FrameDeltaTime ) override;

	virtual void NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
};
