// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapons/CWeaponComponent.h"
#include "Component/CStateComponent.h"
#include "CPlayerAnim.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFootstep, FName, FootBoneName, FVector, HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFootstepWithMaterial, FName, FootBoneName, FVector, HitLocation, UPhysicalMaterial*, HitPhysicalMaterial);

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
	void CheckFootStep();

private:
	void OnWeaponTypeChanged ( EWeaponType InPrevType , EWeaponType InNewType );
	// 발 접지 체크 함수
	void CheckFootstep(FName FootBoneName, bool& bOutIsOnGround, FVector& OutHitLocation, UPhysicalMaterial*& OutHitPhysicalMaterial);

private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;
	class UCStateComponent* State;

public:
	// === 설정 변수 ===

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
	FName LeftFootBoneName="foot_l"; // 왼발 본 이름 (예: "foot_l")

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
	FName RightFootBoneName="foot_r"; // 오른발 본 이름 (예: "foot_r")

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
	float TraceZOffset=20.0f; // 트레이스 시작점의 Z 오프셋 (발목 위로)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
	float TraceLength=35.0f; // 트레이스 길이 (발 아래로)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Footstep")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_EngineTraceChannel5; // 트레이스에 사용할 콜리전 채널 (예: ECC_WorldStatic)

	// === 상태 변수 (내부 사용) ===

	// 이전 프레임의 발 접지 상태
	bool bPrevLeftFootOnGround;
	bool bPrevRightFootOnGround;

	// === 이벤트 델리게이트 (블루프린트에서 바인딩) ===
	UPROPERTY(BlueprintAssignable, Category = "Footstep")
	FOnFootstep OnLeftFootstep; // 왼발 접지 이벤트

	UPROPERTY(BlueprintAssignable, Category = "Footstep")
	FOnFootstep OnRightFootstep; // 오른발 접지 이벤트

	UPROPERTY(BlueprintAssignable, Category = "Footstep")
	FOnFootstepWithMaterial OnLeftFootstepWithMaterial; // 재질 포함 왼발 접지 이벤트 (선택 사항)

	UPROPERTY(BlueprintAssignable, Category = "Footstep")
	FOnFootstepWithMaterial OnRightFootstepWithMaterial; // 재질 포함 오른발 접지 이벤트 (선택 사항)

};
