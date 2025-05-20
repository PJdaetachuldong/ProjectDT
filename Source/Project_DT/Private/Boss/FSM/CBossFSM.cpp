// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/FSM/CBossFSM.h"
#include "Boss/CBossEnemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"

UCBossFSM::UCBossFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCBossFSM::BeginPlay()
{
	Super::BeginPlay();

	GetOwnerEnemy();
}

void UCBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//플레이어와의 거리 체크는 후 공격을 정하던가, 후속타 여부를 위해 체크가 지속적으로 일어나야함
	//아니면 함수로 빼서 특정 상황일때만 체크를 해서 여부를 체크하는 것도 나쁘지 않을것 같음

	if ( State != EBossState::IDLE )
	{
		TargetDist = FVector::Dist(MyBoss->Target->GetActorLocation(), MyBoss->GetActorLocation());
	}

	switch ( State )
	{
		case EBossState::IDLE: { IDLEState(); } break;
		case EBossState::CHASE: { CHASEState ( ); } break;
		case EBossState::ATTACK: { ATTACKState ( ); } break;
		case EBossState::DAMAGE: { DAMAGEState ( ); } break;
		case EBossState::BREAK: { BREAKState ( ); } break;
		case EBossState::DIE: { DIEState ( ); }	break;
	}
}

void UCBossFSM::IDLEState()
{
	//일정 시간이 지나면 IDLE에서 CHASE상태가 되게, 나중에 애니메이션 넣어서 노티파이로 변환되게 해주기
	CurIDLETime +=GetWorld()->GetDeltaSeconds();
	if ( CurIDLETime >= IDLETime )
	{
		State = EBossState::CHASE;

		//천천히 걸어온 느낌을 주기 위해 걷는 속도를 조절
		MyBoss->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
	//일정 시간이 지나면 IDLE에서 CHASE상태가 되게, 나중에 애니메이션 넣어서 노티파이로 변환되게 해주기
}

void UCBossFSM::CHASEState()
{
	//플레이어를 향해서 움직이게 만듦
	AI->MoveToLocation(MyBoss->Target->GetActorLocation());

	//거리를 체크해서 거리가 멀 경우
// 	if ( TargetDist >= LongDist )
// 	{
		//거리가 먼 상태가 얼마나 지속되었는지 체크한다
		CurChaseTime += GetWorld()->GetDeltaSeconds();
/*	}*/

	//만약 거리가 먼 상태가 일정 시간 유지되었다면
	if ( CurChaseTime >= DashAttackCooltime )
	{
		//공격 상태로 변환
		State=EBossState::ATTACK;
		//그리고 대쉬 공격이 이루어지도록 공격상태도 변환
		AttackState = EBossATTACKState::DASHATTACK;

		//빠르게 달리는 느낌을 주기 위하여 이동속도 바꾸기
		MyBoss->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

		//뒤에 코드 실행 안되게 리턴
		return;
	}

	//그냥 추적 상태인데 플레이어가 물약 마시는 모션을 할 경우
// 	if ()
// 	{
// 		//공격 상태로 변환하고
// 		FSMComponent->State == EBossState::ATTACK;
// 		//바로 원거리 공격하게 변환
// 		FSMComponent->AttackState == EBossATTACKState::RANGEDATTACK;
// 	}
	//그냥 추적 상태인데 플레이어가 물약 마시는 모션을 할 경우
	//추적 상태에서 거리를 체크해서 거리가 멀 경우
}

void UCBossFSM::ATTACKState()
{
	//공격 상태에서 또다시 여러가지 공격 패턴으로 나뉘어짐
	switch ( AttackState )
	{
	case EBossATTACKState::NONE: { NONEState ( ); } break;
	case EBossATTACKState::RANGEDATTACK: { RANGEDATTACKState ( ); } break;
	case EBossATTACKState::DASHATTACK: { DASHATTACKState ( ); } break;
	case EBossATTACKState::COMBOATTACK: { COMBOATTACKState ( ); } break;
	case EBossATTACKState::COUNTERATTACK: { COUNTERATTACKState ( ); } break;
	case EBossATTACKState::SPATTACK: { SPATTACKState ( ); } break;
	}
}

void UCBossFSM::DAMAGEState()
{
	
}

void UCBossFSM::BREAKState()
{
	//브레이크 상태가 되면 브레이크의 지속시간을 체크함
	CurBreakTime +=GetWorld()->GetDeltaSeconds();

	//지속시간이 끝났다면
	if ( CurBreakTime >= BreakLimitTime )
	{
		//일어선 다음

		//CHASE상태로 변환? ATTACK상태로 변환? 나중에 정하기
		State = EBossState::CHASE;
	}
}

void UCBossFSM::DIEState()
{

}

void UCBossFSM::NONEState()
{
	//가드 게이지를 채워줌
	MyBoss->GuardGage += GetWorld()->GetDeltaSeconds();
	//가드 게이지를 채워줌

	//만약 가드 조건이 충족되었을 경우
	if ( MyBoss->GuardGage <= MyBoss->GuardPlaying )
	{
		//가드 상태로 변화
		AttackState = EBossATTACKState::COUNTERATTACK;
		//초기화
		MyBoss->GuardGage = 0.0f;

		//밑에 코드 안 일어나게 리턴
		return;
	}

	//플레이어가 공격범위 이내일경우
	if ( TargetDist <= MyBoss->AttackRange )
	{
		SetRANGEDATTACKState(FMath::RandRange(0, 2));
	}
}

void UCBossFSM::RANGEDATTACKState()
{
	
}

void UCBossFSM::DASHATTACKState()
{
	//위치 셋팅이 되어있는 상태이면
	if ( IsSetDashAttackLocation )
	{
		//돌진 시간을 업데이트
		DashTimer += GetWorld()->GetDeltaSeconds() / DashDuration;

		//만약 시간이 정해진 시간 보다 커진다면
		if ( DashTimer >= 1.0f)
		{
			//돌진은 완료했다고 bool값 false로 전환
			IsSetDashAttackLocation = false;
			//Navmesh를 이용하여 다시 이동하게 만듦
			MyBoss->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			//상태를 다시 CHASE로 되돌림
			State = EBossState::CHASE;
			//이동 속도도 다시 원래 추적 속도로 되돌려줌
			MyBoss->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			//추적 시간도 초기화
			CurChaseTime = 0.0f;
			//리턴
			return;
		}

		//easeinsine이 적용되도록 계산하게 만듦
		float t = EaseInSine(DashTimer);
		//Lerp를 사용하여 사이를 보간
		FVector NewLocation = FMath::Lerp(StartDashLocation, CalculatedTargetLocation, t);
		//이동하게 함
		MyBoss->SetActorLocation( NewLocation );
	}

	//위치 세팅이 안되어있는 상태이면
	else if ( !IsSetDashAttackLocation )
	{
		//위치를 설정 
		IsSetDashAttackLocation = true;
		DashTimer = 0.0f;
		StartDashLocation = MyBoss->GetActorLocation();

		//타겟의 위치를 설정
		FVector TargetLocation = MyBoss->Target->GetActorLocation();
		//뒤로 더 이동하기 위해 타겟의 방향을 계산
		FVector DirectionToTarget = (TargetLocation - StartDashLocation).GetSafeNormal();

		//플레이어 위치보다 더 뒤로 이동하게 값을 계산함
		CalculatedTargetLocation = TargetLocation + (DirectionToTarget * DashAttackOverDist);

		//Navmesh를 비활성화해줌
		MyBoss->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
}

void UCBossFSM::COMBOATTACKState()
{

}

void UCBossFSM::COUNTERATTACKState()
{

}

void UCBossFSM::SPATTACKState()
{

}

float UCBossFSM::EaseInSine ( float x )
{
	return 1.0f - FMath::Cos((x * PI) / 2.0f);
}

void UCBossFSM::SetRANGEDATTACKState(int32 RandomComboAttack)
{
	AttackState = EBossATTACKState::RANGEDATTACK;

	ComboAttackIndex = RandomComboAttack;
}

void UCBossFSM::GetOwnerEnemy()
{
	//자신의 주인과 AI를 받음
	MyBoss = Cast<ACBossEnemy>(GetOwner());
	AI = Cast<AAIController>(MyBoss->GetController());
}

