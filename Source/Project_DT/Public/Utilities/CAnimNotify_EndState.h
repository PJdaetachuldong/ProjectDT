// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Component/CStateComponent.h"
#include "CAnimNotify_EndState.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_DT_API UCAnimNotify_EndState : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY ( EditAnywhere , Category = "Type" )
	EStateType StateType;

public:
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* Meshcomp , UAnimSequenceBase* Animation ) override;
};
