// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Familiar/CFamiliarBase.h"
#include "Enemy/EnemyBase/CEnemyBase.h"
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
	 
public:	//소켓 추가
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* AttCollisionBite;

public:	// 스탯 관련
	float AttackDelayTime = 2.f;	// 공격 쿨타임
	float SearchRange = 2000.f;

	float MoveMentSpeed = 300.f;
	float MaxDistance = 400.f;
	float MinDistance = 200.f;

	float AttackRange = MinDistance;

	int32 SpecialMax = 2;
	int32 SpecialStack = 0;		// 맥스 스텟만큼 쌓이면 스페셜 발동
								// 나중에는 플레이어가 발동하는 스킬로 바꿀 예정

public:	// 스탯 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	float MAXHP = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	float hp = MAXHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	float MeleeBiteDMG = 30.f;	// 물기 데미지

public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Check)
	//bool IsLand = false;

	bool IsCanAttack = false;
	bool IsSpawned = false;		// 스폰 상태인지 확인하는 변수
	bool IsInBattle = false;	// 전투 상태인지 확인하는 변수
	bool IsFar = false;			// Idle 상태에서 거리가 먼지 판단하는 변수
	bool IsClose = false;		// Jump 상태에서 거리 판단하는 변수
	bool IsOnBiteAtt = false;	// 공격 쿨타임 되면 true로 변경.
	bool IsOnSpecialAtt = false;

public://오버라이드 할 것 같은 항목
	// virtual void OnWeaponChanged ( );	// 무기에 따른 소환수 변화
	// virtual void AssistAttack ( );	// 보조 공격
	// virtual void SetTarget ( );		// 타겟 지정

public:
	void InitBoxes();

public:
	void SetOnDesPawn( );
	void OnAttOffProcess();		// 공격 관련 bool값 전부 off

public:	// 기타 함수
	void Landed ( const FHitResult& Hit );	// 착지 확인 델리게이트

public:	// 공격 처리 함수
	TArray<ACEnemyBase*> HitPawn;

	UFUNCTION()	
	void OnAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
