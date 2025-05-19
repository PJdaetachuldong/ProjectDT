// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimnotifyState_Collision.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_DT_API UCAnimnotifyState_Collision : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;

	virtual void NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration ) override;
	virtual void NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
};
