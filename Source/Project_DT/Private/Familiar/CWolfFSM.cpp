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
	// ====== 소환 -> 탐지 -> 공격

// 소환 파트
	mUpState = EUpperState::Start;

// 탐지 파트
	SearchEnemy();

}

void UCWolfFSM::IdleState ( )
{

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
	}
}
