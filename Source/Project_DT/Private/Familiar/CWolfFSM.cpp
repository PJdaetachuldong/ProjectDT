// Fill out your copyright notice in the Description page of Project Settings.

// (0514)!!!!! EnemyListUpDate 항목에서 EnemyClass로 변경 필요

#include "Familiar/CWolfFSM.h"
#include "Familiar/CFamiliarWolf.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CPlayer.h"
#include "Enemy/EnemyBase/CEnemyBase.h"
#include "Familiar/CWolfAnimInstance.h"


UCWolfFSM::UCWolfFSM ( )
{
	PrimaryComponentTick.bCanEverTick = true;


}
// -------------------------------------------------
void UCWolfFSM::BeginPlay ( )
{
	Super::BeginPlay ( );

	Me = Cast<ACFamiliarWolf>( GetOwner() );
	if ( !Me ) { return; }

	Anim = Cast<UCWolfAnimInstance> ( Me->GetMesh ( )->GetAnimInstance ( ) );
	
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ACPlayer::StaticClass());
	if(!actor) { return; }
	Player = Cast<ACPlayer>(actor);

}
// -------------------------------------------------
void UCWolfFSM::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );

#pragma region LogMessageState

	FString logMsgUpState = UEnum::GetValueAsString( MUpState );
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Yellow, logMsgUpState );

#pragma endregion LogMessageState

// 최상위 State
	switch ( MUpState )
	{
		case EUpperState::None:			{ }	break;
		case EUpperState::Start:		{ }	break;		
		case EUpperState::Idle:			{ IdleState(); }	break;
		case EUpperState::Jump:			{ JumpState(); }	break;
		case EUpperState::Attack:		{ AttackState(); }	break;
		case EUpperState::CC:			{ }	break;
		case EUpperState::Uncontrolled: { }	break;
	}

	switch ( MJumpState )
	{
	case EJumpState::None:		{ }	break;
	case EJumpState::StopJump:	{ }	break;

	case EJumpState::FrontJump: { }	break;
	case EJumpState::BackJump:	{ }	break;
	case EJumpState::LeftJump:	{ }	break;
	case EJumpState::RightJump: { }	break;
	}

	switch ( MAttState )
	{
		case EAttackState::None:	{ }	break;
		case EAttackState::Attack1:	{ }	break;
	}

}

// ===================== 업데이트 함수 =====================
#pragma region Update_States
void UCWolfFSM::UpdateState ( EUpperState _UpState )
{
	MUpState = _UpState;
	Anim->AUpState = _UpState;
}

void UCWolfFSM::UpdateState ( EJumpState _JumpState )
{
	MJumpState = _JumpState;
	Anim->AJumpState = _JumpState;

}

void UCWolfFSM::UpdateState ( EAttackState _AttState )
{
	MAttState = _AttState;
	Anim->AAttState = _AttState;
}

void UCWolfFSM::UpdateState ( EOverridenState _OverState )
{
	MOverState = _OverState;
	Anim->AOverState = _OverState;
}
#pragma endregion Update_States


// ========================================================================================

void UCWolfFSM::SpawnFamiliar ( )
{
	// ========= 소환 -> 탐지 -> Idle =========

// 소환 파트
	Me->IsSpawned = true;
	UpdateState( EUpperState::Start );

// 탐지 파트	// 바로 공격할 때 사용. 대상 지정 필요할때마다 계속 갱신
	SearchEnemy();

// Idle은 애니메이션 노티파이로 넘길 예정  -  완료
}

void UCWolfFSM::IdleState ( )
{
// ========= 탐지 -> 회전 -> 공격판단
//일정 시간마다 실행
	CurrentTime += GetWorld()->DeltaTimeSeconds;

// 탐지, 대상 지정
	SearchEnemy ( );
	//타겟이 없을 경우에는 플레이어 쫓아다니는 함수.
	if ( Me->IsInBattle == false ) { MoveToTarget(Player); }

	// 타겟이 있을 경우에는 공격모드.
	else
	{
		if ( CurrentTime > Me->AttackDelayTime )
		{
			UpdateState(EUpperState::Attack );
		}
	}
}

void UCWolfFSM::JumpState ( )
{
	//점프 두 번 갈길 경우엔 Bool형 체크도 해주기
	if ( Me->CanJump() )
	{
		Me->Jump();
	}
}

void UCWolfFSM::AttackState ( )
{
	// 회전
		//TurnToTarget ( TargetEnemy );
		//OnAttackProcess ( );
	// 공격 실행은 노티파이로, 점프공격 애니메이션이 있으면 좋을듯. 
		// (없으면 물기 + Launch)
}

void UCWolfFSM::UpdateEnemyList ( )
{
	// 에너미 리스트 우선 비우기
	EnemyList.Empty();

	// !! (현재는 테스트용임!!) 로직 고치기 !!
	// ㄴ범위 내의 에너미 스윕하는 방식으로 하는게 나을듯

	TArray<AActor*> allEnemy;	// 범위 내, 혹은 다른 로직의 에너미 리스트. 한 군집이 베스트.
	UGameplayStatics::GetAllActorsOfClass ( GetWorld ( ) , ACEnemyBase::StaticClass ( ) , allEnemy );

	for ( AActor* allActor : allEnemy )
	{
		ACEnemyBase* enemy = Cast<ACEnemyBase> ( allActor );

		float dist = FVector::Dist ( enemy->GetActorLocation ( ), Me->GetActorLocation ( ) );

		if ( IsValid ( enemy ) )
		{
			// 타겟과의 거리가 색적범위보다 가까울 경우
			if ( dist < Me->SearchRange )
			{
				// 타겟을 리스트에 추가
				EnemyList.Add ( enemy );
			}
		}
	}
}

void UCWolfFSM::Dissolved ( )
{

}

void UCWolfFSM::SearchEnemy ( )
{
	// 플레이어가 공격한 에너미로 타게팅 (플레이어쪽 리스트 필요)
		// 아직 히트된 에너미 리스트가 없다.. 임시로 함수(SetOnRandTarget) 지정.

	// 없다면 플레이어 에너미 리스트 재탐색
		// 현재는 랜덤.
	UpdateEnemyList ( );
	
	// 있을 경우 공격 상태로 전환
	if (!EnemyList.IsEmpty())
	{
		Me->IsInBattle = true;
		SetOnTarget();
	}

	else
	{
		Me->IsInBattle = false;
	}
}

// 전투 상태인 경우에만 실행
void UCWolfFSM::SetOnTarget ( )
{
	UpdateEnemyList ( );
	if ( EnemyList.IsEmpty ( ) ) { return; }	// 에너미 리스트가 비어있다면 실행할 필요가 없음
	HitActors.Empty ( );	// (본인이) 공격했던 타겟 목록 비우기

	TargetEnemy = nullptr;

// Enemy에 가장 가까운 대상 값 넣어주기
	SetOnRandTarget ( );	// 임시 랜덤 타겟 (하단 우선순위 만들시 변경예정)

// 1순위 - 플레이어가 공격중인 대상.
	// ㄴ 플레이어 리스트 받아와야함.

// 2순위 - 플레이어를 공격중인 대상.
	// ㄴ 에너미 상태(공격중인지 불값 등) 받아와야함.

}

void UCWolfFSM::SetOnRandTarget ( )
{
	int32 randTarget = FMath::RandRange ( 0 , EnemyList.Num ( ) - 1 );
	TargetEnemy = EnemyList[randTarget];
}

FVector UCWolfFSM::TargetDir ( AActor* target )
{	
	if ( target == nullptr || Me == nullptr){ return FVector::ZeroVector;}
	
	FVector destination = target->GetActorLocation ( );
	FVector dir = destination - Me->GetActorLocation ( );

	return dir;
}


void UCWolfFSM::TurnToTarget ( AActor* target )
{
//현재 방향 ~ 타겟 방향 각도 구하기
	float deltaTime = GetWorld ( )->DeltaTimeSeconds;
	FVector dir = TargetDir( target );

	FRotator targetRotation = dir.Rotation ( );
	FRotator currentRotation = Me->GetActorRotation ( );


	//float targetYaw = FMath::Abs ( FMath::FindDeltaAngleDegrees ( currentRotation.Yaw , targetRotation.Yaw ) );


	FRotator newRotation = FMath::RInterpTo ( currentRotation , targetRotation , deltaTime , 3.f );
	FRotator finalRot = FRotator ( newRotation.Pitch , newRotation.Yaw , 0.f );

	// 각도 크기만큼 돌리기
	Me->SetActorRotation ( finalRot );

	
/*
	// 아래는 에너미일 경우에만 실행한다.
	if ( target->IsA ( ACEnemyBase::StaticClass()))
	{
	// & 점프 애니메이션 재생 
		//애니메이션 재생은 에셋 넣고 나서.

		// BattleIdle 상태일 경우에만 실행
		//에너미가 특정 각도 이상 or 특정 거리 안쪽일 때 점프로 전환
		UpdateState ( EIdleState::Jump );

		// xxxxxxxxxxxxx
		//노티파이로 점프 실행 후 다시 BattleIdle 실행하도록 전환
		// ㄴ **Wolf에서 Landed 델리게이트로 해결함(착지하면 다시 BattleIdle로 변환). 테스트 필요. 
		// ㄴ 이걸로 하면 안됨 노티파이 해야함! 애니메이션으로 해결 볼 것.
	}
*/

}


void UCWolfFSM::OnAttackProcess ( )
{
// 공격중인 상태로 변환
	UpdateState ( EUpperState::Attack );

	FVector dir = TargetDir ( TargetEnemy );
	DecideAttack();

	UpdateState ( MAttState );
}

void UCWolfFSM::DecideAttack ( )
{

// 세부 공격 타입 결정
	MAttState = EAttackState::Attack1;	// 어떤 공격할지 바꾸기. 일단 물기 하나 넣고 테스트
	
// 실행은 OnAttackProcess 에서

}

void UCWolfFSM::MoveToTarget ( AActor* target )
{
// 타겟과의 사거리 체크
	FVector dirToTarget = target->GetActorLocation ( ) - Me->GetActorLocation ( );
	float distToTarget = dirToTarget.Size ( );

// 타겟이 플레이어일 경우
	if ( target->IsA ( ACPlayer::StaticClass ( ) ) )
	{
		// 사거리가 Max보다 멀어질 경우에는 IsFar = true / Min에 도달하면 false;
		if ( distToTarget > Me->MaxDistance ) { Me->IsFar = true; }
		else if ( distToTarget < Me->MinDistance ) { Me->IsFar = false; }

		// 설정 거리보다 멀다면 플레이어 방향으로 이동.
		if ( Me->IsFar == true )
		{
			dirToTarget.Normalize ( );

			TurnToTarget ( target );
			Me->AddMovementInput ( dirToTarget , 1.0f );
		}
	}

// 타겟이 에너미일 경우
	else if ( target->IsA ( ACEnemyBase::StaticClass ( ) ) )
	{
		//사거리 범위까지 이동 & 회전
		if ( distToTarget < Me->MinDistance )
		{
			dirToTarget.Normalize ( );

			TurnToTarget ( target );
			Me->AddMovementInput ( dirToTarget , 1.0f );

			OnAttackProcess();
		}

	}




}

