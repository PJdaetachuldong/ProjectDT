// Fill out your copyright notice in the Description page of Project Settings.

// (0514)!!!!! EnemyListUpDate 항목에서 EnemyClass로 변경 필요

#include "Familiar/CWolfFSM.h"
#include "Familiar/CFamiliarWolf.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CPlayer.h"


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
	player = Cast<ACPlayer>(actor);

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
	case EUpperState::Start: { StartState ( ); }	break;
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

void UCWolfFSM::StartState ( )
{
}

void UCWolfFSM::IdleState ( )
{
}

void UCWolfFSM::UpdateEnemyList ( )
{
	EnemyList.Empty();

	TArray<AActor*> AllEnemy;	// 범위 내, 혹은 다른 로직의 에너미 리스트. 한 군집이 베스트.
	UGameplayStatics::GetAllActorsOfClass ( GetWorld ( ) , ACPlayer::StaticClass ( ) , AllEnemy );

	for ( AActor* allActor : AllEnemy )
	{
		ACPlayer* enemy = Cast<ACPlayer> ( allActor );

		if (IsValid( enemy ))
		{
			EnemyList.Add(enemy);
		}
	}
}
