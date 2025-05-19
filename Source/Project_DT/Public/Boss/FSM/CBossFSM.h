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
	DIE UMETA(DisplayName = "DIE"),
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

	//어느 콤보 공격을 할지 저장하는 변수
	int32 ComboAttackIndex;

	//콤보공격이 될때 어떤 콤보 공격을 하지 같이 받는 함수
	void SetRANGEDATTACKState(int32 RandomComboAttack);

	//보스 FSM 관련 함수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	EBossState State = EBossState::IDLE;

	void IDLEState();
	void CHASEState();
	void ATTACKState();
	void DAMAGEState();
	void DIEState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	EBossATTACKState AttackState = EBossATTACKState::NONE;

	void NONEState();
	void RANGEDATTACKState();
	void DASHATTACKState();
	void COMBOATTACKState();
	void COUNTERATTACKState();
	void SPATTACKState();

};
