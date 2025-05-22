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
	class UCWolfFSM* WolfFSM;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* WolfComponent;

	UPROPERTY()
	class UCWolfAnimInstance* WolfAnim;

public:	// 스탯 관련
	float AttackDelayTime = 2.f;	// 공격 쿨타임
	float SearchRange = 1000.f;

public:
	bool IsCanAttack = false;

public://오버라이드 할 것 같은 항목
	// virtual void OnWeaponChanged ( );	// 무기에 따른 소환수 변화
	// virtual void AssistAttack ( );	// 보조 공격
	// virtual void SetTarget ( );		// 타겟 지정

public:
	void SetOnDesPawn( );

public:	// 기타 함수
	// void Landed ( const FHitResult& Hit );	// 착지 확인 델리게이트
};
