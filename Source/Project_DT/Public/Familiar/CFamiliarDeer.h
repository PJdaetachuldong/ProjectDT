// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Familiar/CFamiliarBase.h"
#include "CFamiliarDeer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API ACFamiliarDeer : public ACFamiliarBase
{
	GENERATED_BODY()
	
		
public:
	// Sets default values for this character's properties
	ACFamiliarDeer ( );

protected:
	virtual void BeginPlay ( ) override;

public:
	virtual void Tick ( float DeltaTime ) override;

public:	// 기본 세팅 파트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class UCDeerFSM* DeerFSM;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* DeerComponent;

	UPROPERTY()
	class UCDeerAnimInstance* DeerAnim;

public:	// 스탯 관련
	float AttackDelayTime = 2.f;	// 공격 쿨타임
	float SearchRange = 1000.f;
};
