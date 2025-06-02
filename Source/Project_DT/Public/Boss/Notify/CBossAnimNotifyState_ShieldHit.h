// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CBossAnimNotifyState_ShieldHit.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCBossAnimNotifyState_ShieldHit : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
};
