// Fill out your copyright notice in the Description page of Project Settings.

// (0514)!!!!! EnemyListUpDate 항목에서 EnemyClass로 변경 필요

#include "Familiar/CWolfFSM.h"
#include "Familiar/CFamiliarWolf.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CPlayer.h"
#include "Enemy/EnemyBase/CEnemyBase.h"


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
	//case EUpperState::Start: { }	break;		// 소환이 이루어질 때 Spawn 함수를 호출하는게 나을듯.
		case EUpperState::Idle:			{ }	break;
		case EUpperState::Move:			{ }	break;
		case EUpperState::Attack:		{ }	break;
		case EUpperState::CC:			{ }	break;
		case EUpperState::Uncontrolled: { }	break;
	}

	switch ( MIdleState )
	{
		case EIdleState::None:		{ }	break;
		case EIdleState::Idle:		{ }	break;
		case EIdleState::BattleIdle:{ }	break;
		case EIdleState::Jump:		{ JumpState(); } break;
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
		case EAttackState::Attack:	{ }	break;
	}


}
// ========================================================================================

void UCWolfFSM::SpawnFamiliar ( )
{
	// ========= 소환 -> 탐지 -> Idle =========

// 소환 파트
	IsSpawned = true;
	MUpState = EUpperState::Start;

// 탐지 파트	// 바로 공격할 때 사용. 대상 지정 필요할때마다 계속 갱신
	SearchEnemy();

// Idle은 애니메이션 노티파이로 넘길 예정
}

void UCWolfFSM::IdleState ( )
{
// ========= 탐지 -> 회전 -> 공격판단
//일정 시간마다 실행
	CurrentTime += GetWorld()->DeltaTimeSeconds;

// 탐지, 대상 지정
	SearchEnemy ( );
// 회전
	TurnToTarget ();

	if ( CurrentTime > Me->AttackDelayTime )
	{
		MUpState = EUpperState::Attack;		// 공격중인 상태
		MAttState = EAttackState::Attack;	 // 어떤 공격할지 바꾸기

		// 공격 실행은 노티파이로, 점프공격 애니메이션이 있으면 좋을듯. 
			// (없으면 물기 + Launch)
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
		IsInBattle = true;
		SetOnTarget();
	}
}

// 전투 상태인 경우에만 실행
void UCWolfFSM::SetOnTarget ( )
{
	TargetEnemy = nullptr;

	UpdateEnemyList ( );
	if ( EnemyList.IsEmpty ( ) ) { return; }	// 에너미 리스트가 비어있다면 실행할 필요가 없음
	HitActors.Empty ( );	// (본인이) 공격했던 타겟 목록 비우기

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

FVector UCWolfFSM::TargetDir ( )
{	
	if ( TargetEnemy == nullptr || Me == nullptr){ return FVector::ZeroVector;}
	
	FVector destination = TargetEnemy->GetActorLocation ( );
	FVector dir = destination - Me->GetActorLocation ( );

	return dir;
}

void UCWolfFSM::TurnToTarget ( )
{
//현재 방향 ~ 에너미 방향 각도 구하기
	float deltaTime = GetWorld ( )->DeltaTimeSeconds;
	FVector dir = TargetDir();

	FRotator targetRotation = dir.Rotation ( );
	FRotator currentRotation = Me->GetActorRotation ( );

	float targetYaw = FMath::Abs ( FMath::FindDeltaAngleDegrees ( currentRotation.Yaw , targetRotation.Yaw ) );

// 각도 크기만큼 돌리기 & 점프 애니메이션 재생 
	Me->SetActorRotation ( targetRotation );
	//애니메이션 재생은 에셋 넣고 나서.

	// BattleIdle 상태일 경우에만 실행
	//에너미가 특정 각도 이상 or 특정 거리 안쪽일 때 점프로 전환
	MIdleState = EIdleState::Jump;

	// xxxxxxxxxxxxx
	//노티파이로 점프 실행 후 다시 BattleIdle 실행하도록 전환
	// ㄴ **Wolf에서 Landed 델리게이트로 해결함(착지하면 다시 BattleIdle로 변환). 테스트 필요. 
	// ㄴ 이걸로 하면 안됨 노티파이 해야함! 애니메이션으로 해결 볼 것.
}
