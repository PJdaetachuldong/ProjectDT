// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase/CEnemyBase.h"
#include "CBossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API ACBossEnemy : public ACEnemyBase
{
	GENERATED_BODY()
	
public:
	ACBossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//원거리 공격 오브젝트 할당
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACRangeAttack> RangedAttackFactory;

	//원거리 공격 오브젝트를 Object Pool로 관리
	//오브젝트의 최초 생성 수 (혹시몰라 여유분까지)
	int32 MaxRangedAttackCount = 4;

	//원거리 공격 오브젝트 목록
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	TArray<class ACRangeAttack*> RangedAttackList;

	//임의로 하는 필살기 패턴 테스트
	float CurTestSPTime = 0.0f;
	float TestSPTime = 15.0f;
	//임의로 하는 필살기 패턴 테스트

// 	//플레이어와의 거리를 저장하는 변수
// 	float TargetDist = 0.0f;
// 
// 	//어느 거리가 먼 거리인지 설정하는 변수
// 	float LongDist = 700.0f;
// 
// 	//거리가 먼 상태에서 시간이 얼마나 지났는지 저장하는 변수
// 	float CurChaseTime = 0.0f;
// 
// 	//대쉬 공격이 실행되는 쿨타임 변수
// 	float DashAttackCooltime = 25.0f;
	
	//콤보 공격을 저장하는 변수
	
	
	//가드 조건 게이지
	float GuardGage = 0.0f;

	//가드가 실행되는 조건 설정 변수
	float GuardPlaying = 10.0f;

	//가드가 성공하였는지 체크하는 변수
	bool IsSucssessGuard = false;

	//준비 자세일때 받은 데미지의 총량을 저장하는 변수
	float OnSPDamage = 0.0f;

	//필살기 준비 자세때 패턴이 파훼될때까지 필요한 데미지를 설정하는 변수
	float SPBreakDamageAmount = 150.0f;

	void SPBreak();

	//일단 임시로 하는 발사 위치 설정
	UPROPERTY(EditAnywhere)
	class USceneComponent* ThrowPosition;

	// FSM 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCBossFSM* FSMComponent;
	
	virtual void EnemyHitDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
