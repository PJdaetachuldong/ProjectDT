// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Familiar/CFamiliarBase.h"
#include "CFamiliarWolf.generated.h"

UCLASS()
class PROJECT_DT_API ACFamiliarWolf : public ACFamiliarBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ACFamiliarWolf ( );

protected:
	virtual void BeginPlay ( ) override;

public:
	virtual void Tick ( float DeltaTime ) override;

public:	// 기본 세팅 파트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class UCWolfFSM* FSM;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* WolfComponent;

	UPROPERTY()
	class UCWolfAnimInstance* Anim;

public:	// 스탯 관련
	float AttackDelayTime = 2.f;	// 공격 쿨타임
	float SearchRange = 2000.f;

	float MoveMentSpeed = 300.f;
	float MaxDistance = 400.f;
	float MinDistance = 200.f;

	float AttackRange = MinDistance;

public:
	bool IsCanAttack = false;

	bool IsSpawned = false;		// 스폰 상태인지 확인하는 변수
	bool IsInBattle = false;	// 전투 상태인지 확인하는 변수
	bool IsFar = false;			// Idle 상태에서 거리가 먼지 판단하는 변수

public://오버라이드 할 것 같은 항목
	// virtual void OnWeaponChanged ( );	// 무기에 따른 소환수 변화
	// virtual void AssistAttack ( );	// 보조 공격
	// virtual void SetTarget ( );		// 타겟 지정

public:
	void SetOnDesPawn( );

public:	// 기타 함수
	void Landed ( const FHitResult& Hit );	// 착지 확인 델리게이트
};
