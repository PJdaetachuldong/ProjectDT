// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/ACGhostTrail.h"
#include "CAnimNotifyState_GhostTrail.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_DT_API UCAnimNotifyState_GhostTrail : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Action")
	TSubclassOf<class AACGhostTrail> GhostTrailClass;

private:
	class AACGhostTrail* GhostTrail;
};
