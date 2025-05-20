// Fill out your copyright notice in the Description page of Project Settings.


#include "Familiar/CWolfAnimInstance.h"
#include "Familiar/CFamiliarWolf.h"
#include "Familiar/CWolfFSM.h"

void UCWolfAnimInstance::NativeUpdateAnimation ( float DeltaSeconds )
{
	Me = Cast<ACFamiliarWolf> ( TryGetPawnOwner ( ) );
	if ( !Me ) { return; }

	FSM = Me->FindComponentByClass<UCWolfFSM> ( );

	FVector velocity = Me->GetVelocity ( );
	FVector forwardVector = Me->GetActorForwardVector ( );


	Speed = FVector::DotProduct ( velocity , forwardVector );

	FVector rightVector = Me->GetActorRightVector ( );
	Direction = FVector::DotProduct ( velocity , rightVector );

}
// ===================== 업데이트 함수 =====================

#pragma region UpDate_States
void UCWolfAnimInstance::NewState ( EUpperState _UpState )
{
	FSM->MUpState = _UpState;
	AUpState = _UpState;
}

void UCWolfAnimInstance::NewState ( EIdleState _IdleState )
{
	FSM->MIdleState = _IdleState;
	AIdleState = _IdleState;
}

void UCWolfAnimInstance::NewState ( EJumpState _JumpState )
{
	FSM->MJumpState = _JumpState;
	AJumpState = _JumpState;

}

void UCWolfAnimInstance::NewState ( EAttackState _AttState )
{
	FSM->MAttState = _AttState;
	AAttState = _AttState;
}

void UCWolfAnimInstance::NewState ( EOverridenState _OverState )
{
	FSM->MOverState = _OverState;
	AOverState = _OverState;
}
#pragma endregion UpDate_States

// ===================== 노티파이 추가하는 파트 =====================

void UCWolfAnimInstance::AnimNotify_Roar_END ( )
{
	if ( !Me ) { return; }
	NewState(EUpperState::Idle);
}
