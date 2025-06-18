#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBossFSM.generated.h"

UENUM()
enum class EBossState : uint8
{
	IDLE UMETA(DisplayName = "IDLE"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	DAMAGE UMETA(DisplayName = "DAMAGE"),
	BREAK UMETA ( DisplayName = "BREAK" ),
	DIE UMETA(DisplayName = "DIE")
};

UENUM ()
enum class EBossATTACKState : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	SETATK UMETA(DisplayName = "SETATK"),
	RANGEDATTACK UMETA(DisplayName = "RANGEDATTACK"),
	DASHATTACK UMETA(DisplayName = "DASHATTACK"),
	COMBOATTACK UMETA(DisplayName = "COMBOATTACK"),
	COUNTERATTACK UMETA(DisplayName = "COUNTERATTACK"),
	SPATTACK UMETA(DisplayName = "SPATTACK")
};

UENUM()
enum class ESetATKState : uint8
{
	SETATKNONE UMETA(DisplayName = "SETATKNONE"),
	BACKSTEP UMETA(DisplayName = "BACKSTEP"),
	SIDEMOVE UMETA(DisplayName = "SIDEMOVE"),
	FRONTMOVE UMETA(DisplayName = "FRONTMOVE")
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
	float LongDist = 550.0f;

	//거리가 먼 상태에서 시간이 얼마나 지났는지 저장하는 변수
	float CurChaseTime = 0.0f;

	//공격 상태에서 이동한 거리를 저장하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalMoveDistance = 0.0f;

	//마지막 있던 위치
	FVector LastLocation;

	//좌 또는 우로 움직이게 하는 변수
	void SideMove();

	//좌우 이동 위치
	FVector TargetSideLocation;

	//좌우 이동이 정해졌다고 체크하는 불
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSideMoveSetting = false;

	//원거리 공격 오브젝트를 소환하는 함수
	void SpawnRangedActor();

	//원거리 공격 몇번했는지 체크함
	int32 RangedAttackCount = 0;

	//대쉬 공격이 실행되는 쿨타임 변수
	float DashAttackCooltime = 5.0f;

	//대쉬 공격 때 플레이어의 얼마나 뒤로 움직일지 정하는 변수
	float DashAttackOverDist = 700.0f;

	//대쉬 공격 때 플레이어보다 약간 앞에서 멈추게 하는 변수
	float DashAttackFrontDist = 50.0f;

	//대쉬 공격을 할때 위치 선정을 했는지 확인하는 변수
	bool IsSetDashAttackLocation = false;

	//애니메이션 대쉬 공격 준비가 끝났는지 확인하는 bool
	bool IsReadyDashAttack = false;

	//대쉬 공격 이동중 사거리가 별로 차이가 안 나는지 확인하는 불
	bool IsLowDist = false;

	//대쉬 지속 시간
	float DashDuration = 1.0f;

	//대쉬 공격 타이머
	UPROPERTY(VisibleAnywhere)
	float DashTimer = 0.0f;

	//대쉬공격을 하는 시작 위치
	FVector StartDashLocation;

	//타겟의 위치
	FVector CalculatedTargetLocation;

	//대쉬 공격 위치를 설정하는 함수
	void SetDashAttackLocation();

	//EaseInSine 계산을 하는 함수
	float EaseInSine(float x);

	//필살기 준비자세일 때 받는 데미지의 총량을 저장하는 변수
	float OnSpDamageAmount = 0.0f;

	//필살기 준비 자세때 패턴이 파훼될때까지 필요한 데미지를 설정하는 변수
	float SPBreakDamageAmount = 50.0f;

	//처음 필살기를 사용했는지 체크하는 불
	bool IsFirstSPAttack = false;

	//현재 공격 스택
	int32 CurAttackStack = 0;

	//필살기 공격 사용가능하도록 만드는 스택
	int32 SPAttackStack = 30;

	//필살기 준비자세일 때 받은 데미지를 받게하는 변수
	void SetSPDamage(float Damage);

	//콤보 공격 실행 조건 변수
	float CurComboAttackTime = 0.0f;

	//콤보 공격 쿨타임, 적어야함
	float ComboCooltime = 3.5f;

	//어느 콤보 공격을 할지 저장하는 변수
	int32 ComboAttackIndex;

	//플레이어 뒷 위치 저장
	FVector PlayerBackLocation;

	//콤보공격이 될때 어떤 콤보 공격을 하지 같이 받는 함수
	void SetCOMBOATTACKState(int32 RandomComboAttack);

	//해당 값보다 클 경우 가운데로 가게 만드는 변수
	float CenterDist = 1800.0f;
	
	//시작될때 자신의 주인과 AI를 받게하는 함수
	void GetOwnerEnemy();

	float CurSideMoveTime = 0.0f;
	float SideMoveLimitTime = 3.5f;
	float SideDirection = 0.0f;

	//자신의 주인과 AI를 저장하는 변수
	class ACBossEnemy* MyBoss;
	class AAIController* AI;

	//보스 FSM 관련 함수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	EBossState State = EBossState::IDLE;

	void IDLEState();
	void ATTACKState();
	void DAMAGEState();
	void BREAKState();
	void DIEState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	EBossATTACKState AttackState = EBossATTACKState::NONE;

	void NONEState();
	void SETATKState();
	void RANGEDATTACKState();
	void DASHATTACKState();
	void COMBOATTACKState();
	void COUNTERATTACKState();
	void SPATTACKState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	ESetATKState SetATKState = ESetATKState::SETATKNONE;

	void SETATKNONEState();
	void BACKSTEPState();
	void SIDEMOVEState();
	void FRONTMOVEState();

	FTimerHandle SetSIDEMOVEStateTimerHandle;
	void SetSIDEMOVEState();

	float ParringDontMoveTime = 2.5f;
	float ParringDibtMoveLimitTime = 2.5f;
};
