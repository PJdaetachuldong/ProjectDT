// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapons/CWeaponComponent.h"
#include "Component/CStateComponent.h"
#include "CPlayerAnim.generated.h"

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
	UPROPERTY(BlueprintReadOnly,Category="Anim" )
	bool isMontagePlaying=false;
protected:
	UPROPERTY ( BlueprintReadOnly , EditAnywhere , Category = "Animation" )
	EWeaponType WeaponType = EWeaponType::Max;
	UPROPERTY ( BlueprintReadOnly , EditAnywhere , Category = "Animation" )
	EStateType StateType = EStateType::Max;
	UPROPERTY ( BlueprintReadOnly , EditAnywhere , Category = "Animation" )
	bool isGuardHit;


public:
	void NativeBeginPlay ( ) override;
	void NativeUpdateAnimation ( float DeltaSeconds ) override;

private:
	void OnWeaponTypeChanged ( EWeaponType InPrevType , EWeaponType InNewType );

private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;
	class UCStateComponent* State;

};
