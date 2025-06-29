// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_HitCamearaShake.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCAnimNotify_HitCamearaShake : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake")
	float ShakeScale = 1.0f;
};
