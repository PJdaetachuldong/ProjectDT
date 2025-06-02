// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Familiar/CFamiliarBaseAnimInstance.h"
#include "CWolfFSM.h"
#include "CWolfAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCWolfAnimInstance : public UCFamiliarBaseAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;

public:
	class ACFamiliarWolf* Me;
	class UCWolfFSM* FSM;

public:	// 수치값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM_Move")
	float Speed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM_Move")
	float Direction = 0.f;

public:	// 불형 체크
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM_Move")
	bool IsJumping = false;

public://FSM State 관련
#pragma region State
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EUpperState AUpState = EUpperState::Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EJumpState AJumpState = EJumpState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EAttackState AAttState = EAttackState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EOverridenState AOverState = EOverridenState::None;
#pragma endregion State 



public:	// AnimNotify 관련 -------- UFUNCTION() 잊지말것!!!
	UFUNCTION() 
	void AnimNotify_Roar_END();

	UFUNCTION() 
	void AnimNotify_Search_Target();

	UFUNCTION() 
	void AnimNotify_Dissolve_Start();

	UFUNCTION() 
	void AnimNotify_DesPawn_End();

	UFUNCTION() 
	void AnimNotify_Wolf_Att_Bite_Start();

	UFUNCTION() 
	void AnimNotify_Wolf_Att_Special_Start ();

	UFUNCTION() 
	void AnimNotify_Wolf_Land_End();

public:	// 스위치문에 전문 때려박으면 보기 힘들어서... 따로 빼는용 함수
	UFUNCTION() 
	void AnimNotify_Wolf_Att_End();

	void Attack1 ( );
	void AttackSpecial ( );

};
