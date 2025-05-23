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
	Start			UMETA(DisplayName = "Start"),		
	Idle			UMETA(DisplayName = "Idle"),
	Jump			UMETA(DisplayName = "Jump"),
	Attack			UMETA(DisplayName = "Attack"),
	CC				UMETA(DisplayName = "CC"),
	Uncontrolled	UMETA(DisplayName = "Uncontrolled")
};

//========================= 하위 스테이트 머신 ==================================

UENUM() // HFSM - 공격 상태
enum class EAttackState : uint8
{
	None		UMETA(DisplayName = "None"),
	Attack1		UMETA(DisplayName = "Attack1")
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
	EUpperState MUpState = EUpperState::Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EJumpState MJumpState = EJumpState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EAttackState MAttState = EAttackState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EOverridenState MOverState = EOverridenState::None;
#pragma endregion State 

// 스테이트 업데이트
#pragma region Update_States
public:	// State 변환
	void UpdateState ( EUpperState _UpState );
	void UpdateState ( EJumpState _JumpState );
	void UpdateState ( EAttackState _AttState );
	void UpdateState ( EOverridenState _OverState );
#pragma endregion Update_States

// 본인, 플레이어, 에너미리스트
#pragma region Target
private:
	UPROPERTY()
	class ACFamiliarWolf* Me;
	UPROPERTY()
	class UCWolfAnimInstance* Anim;

	UPROPERTY()
	class ACPlayer* Player;	// 쫓아갈 플레이어

	UPROPERTY()	
	class ACEnemyBase* TargetEnemy = nullptr;		// 타게팅 된 에너미

	UPROPERTY()	
	TArray<ACEnemyBase*>EnemyList;
	TArray<AActor*> HitActors;	// 다단히트 방지
#pragma endregion Target

public:
	float CurrentTime = 0.f;

public:	// State 관련 함수
	void IdleState();
	void JumpState();
	void AttackState();

public:	// 실행쪽 함수
	void SpawnFamiliar();
	void UpdateEnemyList();
	void Dissolved();

public:	// 에너미 색적 
	void SearchEnemy();			// 적 리스트 갱신, IsInBattle 갱신
	void SetOnTarget();			// 타겟 지정하기
	void SetOnRandTarget ( );	// 임시타겟. 리스트 랜덤돌림.


public:	// 공격 실행 함수
	void OnAttackProcess();		// 공격 실행하는 함수
	void EndAttackProcess();	// 공격 관련 정리하는 함수
	void DecideAttack();		// 어떤 공격할지 결정하는 함수

public:	// 이동 관련 함수
	bool CheckPath();
	FVector TargetDir ( AActor* target );		// 타겟 Dir 구하기
	void MoveToTarget( AActor* target );		// 타겟 방향으로 이동하기
	void TurnToTarget( AActor* target );		// 타겟 방향으로 몸 돌리기
};
