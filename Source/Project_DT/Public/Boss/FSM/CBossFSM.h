#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBossFSM.generated.h"

UENUM()
enum class EBossState : uint8
{
	IDLE UMETA(DisplayName = "IDLE"),
	CHASE UMETA(DisplayName = "CHASE"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	DAMAGE UMETA(DisplayName = "DAMAGE"),
	BREAK UMETA ( DisplayName = "BREAK" ),
	DIE UMETA(DisplayName = "DIE")
};

UENUM ()
enum class EBossATTACKState : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	RANGEDATTACK UMETA(DisplayName = "RANGEDATTACK"),
	DASHATTACK UMETA(DisplayName = "DASHATTACK"),
	COMBOATTACK UMETA(DisplayName = "COMBOATTACK"),
	COUNTERATTACK UMETA(DisplayName = "COUNTERATTACK"),
	SPATTACK UMETA(DisplayName = "SPATTACK")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//플레이어와의 거리를 저장하는 변수
	float TargetDist = 0.0f;

	//어느 거리가 먼 거리인지 설정하는 변수
	float LongDist = 700.0f;

	//거리가 먼 상태에서 시간이 얼마나 지났는지 저장하는 변수
	float CurChaseTime = 0.0f;

	//대쉬 공격이 실행되는 쿨타임 변수
	float DashAttackCooltime = 5.0f;

	//대쉬 공격 때 플레이어의 얼마나 뒤로 움직일지 정하는 변수
	float DashAttackOverDist = 1100.0f;

	//대쉬 공격을 할때 위치 선정을 했는지 확인하는 변수
	bool IsSetDashAttackLocation = false;

	//대쉬 지속 시간
	float DashDuration = 1.0f;

	//대쉬 공격 타이머
	UPROPERTY(VisibleAnywhere)
	float DashTimer = 0.0f;

	//대쉬공격을 하는 시작 위치
	FVector StartDashLocation;

	//타겟의 위치
	FVector CalculatedTargetLocation;

	//EaseInSine 계산을 하는 함수
	float EaseInSine(float x);

	//필살기가 파훼되어서 Break 상태의 시간을 저장하는 변수
	float CurBreakTime = 0.0f;

	//Break상태의 지속시간을 설정한 변수
	float BreakLimitTime = 5.0f;

	//어느 콤보 공격을 할지 저장하는 변수
	int32 ComboAttackIndex;

	//콤보공격이 될때 어떤 콤보 공격을 하지 같이 받는 함수
	void SetRANGEDATTACKState(int32 RandomComboAttack);
	
	//시작될때 자신의 주인과 AI를 받게하는 함수
	void GetOwnerEnemy();

	//자신의 주인과 AI를 저장하는 변수
	class ACBossEnemy* MyBoss;
	class AAIController* AI;

	//보스 FSM 관련 함수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	EBossState State = EBossState::IDLE;

	void IDLEState();
	void CHASEState();
	void ATTACKState();
	void DAMAGEState();
	void BREAKState();
	void DIEState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	EBossATTACKState AttackState = EBossATTACKState::NONE;

	void NONEState();
	void RANGEDATTACKState();
	void DASHATTACKState();
	void COMBOATTACKState();
	void COUNTERATTACKState();
	void SPATTACKState();

	//임의로 하는 IDLE에서 CHASE로 넘어가는 타이머
	float CurIDLETime = 0.0f;
	float IDLETime = 6.0f;
	//임의로 하는 IDLE에서 CHASE로 넘어가는 타이머
};
