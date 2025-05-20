// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Familiar/FamiliarBaseFSM.h"
#include "CWolfFSM.generated.h"


#pragma region	EState
// FSM상으론 Idle(시작시 Start 프로세스 후 bIsStart 체크) / Move / Attack / Overriden(CC / PlayerControlled) 네 가지로 분류

UENUM()	// 상위 스테이트 머신
enum class EUpperState : uint8
{
	None			UMETA(DisplayName = "None"),		// 소환 해제 대기상태
	Start			UMETA(DisplayName = "Start"),		// 
	Idle			UMETA(DisplayName = "Idle"),
	Move			UMETA(DisplayName = "Move"),
	Attack			UMETA(DisplayName = "Attack"),
	CC				UMETA(DisplayName = "CC"),
	Uncontrolled	UMETA(DisplayName = "Uncontrolled")
};

//========================= 하위 스테이트 머신 ==================================

UENUM() // HFSM - 대기 상태
enum class EIdleState : uint8
{
	None		UMETA(DisplayName = "None"),
	Idle		UMETA(DisplayName = "Idle"),
	BattleIdle	UMETA(DisplayName = "BattleIdle") ,
	Jump		UMETA(DisplayName = "Jump")
};

UENUM() // HFSM - 공격 상태
enum class EAttackState : uint8
{
	None		UMETA(DisplayName = "None"),
	Attack		UMETA(DisplayName = "Attack")
};

UENUM() // HFSM - 이동 상태 (점프로 각도 조정등 수행)
enum class EJumpState : uint8
{
	None		UMETA(DisplayName = "None"),
	StopJump	UMETA(DisplayName = "StopJump"),

	FrontJump	UMETA(DisplayName = "FrontJump"),
	BackJump	UMETA(DisplayName = "BackJump"),
	LeftJump	UMETA(DisplayName = "LeftJump"),
	RightJump	UMETA(DisplayName = "RightJump")
};

UENUM() // HFSM - 강제 컨트롤 상태
enum class EOverridenState : uint8
{
	None			UMETA(DisplayName = "None"),
	CC				UMETA(DisplayName = "CC"),
	Uncontrolled	UMETA(DisplayName = "Uncontrolled")
};

#pragma endregion EState

UCLASS()
class PROJECT_DT_API UCWolfFSM : public UFamiliarBaseFSM
{

// 기본 생성자, BeginPlay, Tick
#pragma region Base
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCWolfFSM ( );

protected:
	// Called when the game starts
	virtual void BeginPlay ( ) override;

public:
	// Called every frame
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;
#pragma endregion Base

// 스테이트 정의
#pragma region State
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EUpperState mUpState = EUpperState::Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EIdleState mIdleState = EIdleState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EJumpState mJumpState = EJumpState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EAttackState mAttState = EAttackState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EOverridenState mOverState = EOverridenState::None;
#pragma endregion State 


// 본인, 플레이어, 에너미리스트
#pragma region Target
private:
	UPROPERTY()
	class ACFamiliarWolf* Me;
	UPROPERTY()
	class ACPlayer* Player;	// 쫓아갈 플레이어
	UPROPERTY()	
	class ACEnemyBase* TargetEnemy;		// 타게팅 된 에너미

	UPROPERTY()	
	TArray<ACEnemyBase*>EnemyList;
	TArray<AActor*> HitActors;	// 다단히트 방지
#pragma endregion Target

public:
	float CurrentTime = 0.f;

public:
	bool IsSpawned = false;		// 스폰 상태인지 확인하는 변수
	bool IsInBattle = false;	// 전투 상태인지 확인하는 변수

public:	// State 관련 함수
	void IdleState();
	void JumpState();

public:	// 실행쪽 함수
	void SpawnFamiliar();
	void UpdateEnemyList();

	// Start, 공격 직전, 피격
	void SearchEnemy();			// 적 리스트 갱신
	void SetOnTarget();			// 타겟 지정하기
	void SetOnRandTarget ( );	// 임시타겟. 리스트 랜덤돌림.
	FVector TargetDir ();		// 타겟 Dir 구하기
	void TurnToTarget();		// 타겟 방향으로 몸 돌리기


};
