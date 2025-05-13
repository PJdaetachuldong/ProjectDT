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
	Start				UMETA(DisplayName = "Start"),	//시작시에만 잠시 사용
	Idle				UMETA(DisplayName = "Idle"),
	Move				UMETA(DisplayName = "Move"),
	Attack				UMETA(DisplayName = "Attack"),
	CC					UMETA(DisplayName = "CC"),
	Uncontrolled		UMETA(DisplayName = "Uncontrolled")
};

//========================= 하위 스테이트 머신 ==================================

UENUM() // HFSM - 대기 상태
enum class EIdleState : uint8
{
	None		UMETA(DisplayName = "None"),
	Idle		UMETA(DisplayName = "Idle")
};

UENUM() // HFSM - 공격 상태
enum class EAttackState : uint8
{
	None		UMETA(DisplayName = "None"),
	Attack		UMETA(DisplayName = "Attack")
};

UENUM() // HFSM - 이동 상태
enum class EMoveState : uint8
{
	None		UMETA(DisplayName = "None"),
	Move		UMETA(DisplayName = "Move")
};

UENUM() // HFSM - 이동 상태
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

// 본인, 플레이어, 에너미리스트
#pragma region Target
private:
	UPROPERTY()
	class ACFamiliarWolf* me;

	UPROPERTY()
	class ACPlayer* player;	// 쫒아갈 플레이어

	UPROPERTY()	
	TArray<ACPlayer*>EnemyList;	// 에너미 클래스로 변경해야 함. 일단은 플레이어 적어둠
#pragma endregion Target

// 스테이트 정의
#pragma region State
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EUpperState mUpState = EUpperState::Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EIdleState mIdleState = EIdleState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EAttackState mAttState = EAttackState::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	EOverridenState mOverState = EOverridenState::None;

#pragma endregion State 

public:
	void StartState();
	void IdleState ( );
public:
	void UpdateEnemyList();

};
