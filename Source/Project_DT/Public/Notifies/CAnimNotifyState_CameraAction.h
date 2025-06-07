// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_CameraAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCAnimNotifyState_CameraAction : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;

	virtual void NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration ) override;
	virtual void NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector ArmVector = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator ArmRotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SpringArmLength = 200.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Second = 0.4f;
};
