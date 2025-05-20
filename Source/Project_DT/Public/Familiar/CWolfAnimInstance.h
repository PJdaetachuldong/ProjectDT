// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Familiar/CFamiliarBaseAnimInstance.h"
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
	float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM_Move")
	float Direction = 0.f;

	
public:	// AnimNotify 관련 -------- UFUNCTION() 잊지말것!!!

};
