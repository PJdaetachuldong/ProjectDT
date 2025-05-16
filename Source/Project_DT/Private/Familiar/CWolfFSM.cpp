// Fill out your copyright notice in the Description page of Project Settings.

// (0514)!!!!! EnemyListUpDate 항목에서 EnemyClass로 변경 필요

#include "Familiar/CWolfFSM.h"
#include "Familiar/CFamiliarWolf.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CPlayer.h"
#include "Familiar/CDummyForFamiliar.h"


UCWolfFSM::UCWolfFSM ( )
{
	PrimaryComponentTick.bCanEverTick = true;


}
// -------------------------------------------------
void UCWolfFSM::BeginPlay ( )
{
	Super::BeginPlay ( );

	me = Cast<ACFamiliarWolf>( GetOwner() );
	if ( !me ) { return; }

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ACPlayer::StaticClass());
	if(!actor) { return; }
	Player = Cast<ACPlayer>(actor);

}
// -------------------------------------------------
void UCWolfFSM::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );

#pragma region LogMessageState

	FString logMsgUpState = UEnum::GetValueAsString( mUpState );
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Yellow, logMsgUpState );

#pragma endregion LogMessageState

	//state 변경
	switch ( mUpState )
	{
	//case EUpperState::Start: { }	break;		// 소환이 이루어질 때 Spawn 함수를 호출하는게 나을듯.
	case EUpperState::Idle:	 { }	break;
	case EUpperState::Move:	 { }	break;
	case EUpperState::Attack:{ }	break;
	case EUpperState::CC:	 { }	break;
	case EUpperState::Uncontrolled: { }	break;
	}

	switch ( mAttState )
	{
	case EAttackState::None: {}	break;
	case EAttackState::Attack: {}	break;
	}

}
// ========================================================================================

void UCWolfFSM::SpawnFamiliar ( )
{
	// ========= 소환 -> 탐지 -> Idle =========

// 소환 파트
	bIsSpawned = true;
	mUpState = EUpperState::Start;

// 탐지 파트	// 바로 공격할 때 사용. 대상 지정 필요할때마다 계속 갱신
	SearchEnemy();

// Idle은 애니메이션 노티파이로 넘길 예정
}

void UCWolfFSM::IdleState ( )
{
	// ========= 탐지 -> 회전 -> 공격판단
	//일정 시간마다 실행
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	if (CurrentTime > AttackDelayTime )
	{
	}

// 탐지, 대상 지정
	SearchEnemy ( );
// 회전
	SetTargetDir();

}

void UCWolfFSM::UpdateEnemyList ( )
{
	// 에너미 리스트 우선 비우기
	EnemyList.Empty();

	// !! 로직 고치기 !!
	// ㄴ범위 내의 에너미 스윕하는 방식으로 하는게 나을듯

	TArray<AActor*> AllEnemy;	// 범위 내, 혹은 다른 로직의 에너미 리스트. 한 군집이 베스트.
	UGameplayStatics::GetAllActorsOfClass ( GetWorld ( ) , ACPlayer::StaticClass ( ) , AllEnemy );

	for ( AActor* allActor : AllEnemy )
	{
		ACDummyForFamiliar* enemy = Cast<ACDummyForFamiliar> ( allActor );

		if (IsValid( enemy ))
		{
			EnemyList.Add(enemy);
		}
	}
}

void UCWolfFSM::SearchEnemy ( )
{
	// 플레이어를 타게팅 하고있는 에너미가 있는지 확인

	// 해당 에너미를 플레이어가 공격한 적이 있는지 확인 
	// ㄴ bAttacked 등 불값 체크해주면 좋을듯.

	// 없다면 주변에 에너미가 있는지 확인
	UpdateEnemyList ( );

	// 있을 경우 공격 상태로 전환
	if (!EnemyList.IsEmpty())
	{
		bIsInBattle = true;
		SetOnTarget();
	}
}

// 전투 상태인 경우에만 실행
void UCWolfFSM::SetOnTarget ( )
{
	// Enemy에 가장 가까운 대상 값 넣어주기

	// 1순위 - 플레이어가 공격중인 대상.
	// ㄴ 플레이어 리스트 받아와야함.

	// xx2순위 - 어그로 게이지가 가장 높은 대상
	// xxㄴ 에너미 어그로 게이지 받아와야 함.

	// 2순위 - 플레이어를 공격중인 대상.
	// ㄴ 에너미 상태(공격중인지 불값 등) 받아와야함.


}

void UCWolfFSM::SetTargetDir ( )
{	
	// 현재 보는 방향과 에너미 위치 각도 크기 구하기
	// 특정 각도(5~10도) 이상 회전하는 경우 점프 애니메이션 재생 
	// 각도 크기만큼 돌리기

	// BattleIdle 상태일 경우에만 실행
	//에너미가 특정 각도 이상 or 특정 거리 안쪽일 때 점프로 전환
	mIdleState = EIdleState::Jump;

	//노티파이로 점프 실행 후 다시 BattleIdle 실행하도록 전환
}
