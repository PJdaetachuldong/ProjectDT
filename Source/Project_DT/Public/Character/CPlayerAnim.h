// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPlayerAnim.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_DT_API UCPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY ( BlueprintReadOnly , Category = "Anim" )
	float Speed;

	UPROPERTY(BlueprintReadOnly,Category="Anim" )
	class UCharacterMovementComponent* Movement;

	UPROPERTY(BlueprintReadOnly,Category="Anim" )
	class UCTrajectoryComponent* Trajectory;

public:
	void NativeBeginPlay ( ) override;
	void NativeUpdateAnimation ( float DeltaSeconds ) override;

private:
	class ACharacter* OwnerCharacter;


};
