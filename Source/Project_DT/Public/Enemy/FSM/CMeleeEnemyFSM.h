// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../CMeleeEnemy.h"
#include "CMeleeEnemyFSM.generated.h"

UENUM()
enum class EMeleeEnemyState : uint8
{
	IDLE UMETA(DisplayName = "IDLE"),
	CHASE UMETA(DisplayName = "CHASE"),
	WANDER UMETA(DisplayName = "WANDER"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	DAMAGE UMETA(DisplayName = "DAMAGE"),
	DIE UMETA(DisplayName = "DIE"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCMeleeEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCMeleeEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	ACMeleeEnemy* MyEnemy;

	//플레이어 주변 랜덤 이동을 위한것
	FVector AttackRandomLocation;
	bool IsSetAttackRandomLocation;

	float CurMoveTime = 0.0f;
	float CurAttackMoveTime = 0.0f;

	//공격권 없이 주변 배회할때 필요한 설정
	FVector WanderMoveLocation;

	bool MoveClockwise = false;
	float WanderSpeed = 450.0f;
	float WanderRadius = 500.0f;

	bool IsWanderMoveSet = false;

	float CurWanderDelayTime = 7.0f;
	void SetWanderMoveLocation();

	float BackStepDistance = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	EMeleeEnemyState State = EMeleeEnemyState::IDLE;

	void GetOwnerEnemy();

	void LookAtTarget();

	FVector BackstepPosition();

	//임의로 그냥 시간을 재서 다시 MOVE상태가 되게 작성
	float CurTime = 0.0f;
	//임의로 그냥 시간을 재서 다시 MOVE상태가 되게 작성
	UPROPERTY()
	class ACMeleeAIController* AI;

	void IDLEState();
	void CHASEState();
	void WANDERState();
	void ATTACKState();
	void DMAGEState();
	void DIEState();
};
