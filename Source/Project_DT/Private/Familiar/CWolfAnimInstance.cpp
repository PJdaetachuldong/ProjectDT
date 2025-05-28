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

	//IsLand = Me->IsLand;
}


// ===================== 노티파이 추가하는 파트 =====================



void UCWolfAnimInstance::AnimNotify_Roar_END ( )
{
	if ( !FSM ) { return; }
	FSM->UpdateState(EUpperState::Idle);
}

void UCWolfAnimInstance::AnimNotify_Search_Target ( )
{
	if ( !FSM || !Me ) { return; }

	FSM->SearchEnemy ( );
	Me->IsCanAttack = true;
}

void UCWolfAnimInstance::AnimNotify_Dissolve_Start ( )
{
	if ( !FSM ) { return; }

	FSM->Dissolved();
}

void UCWolfAnimInstance::AnimNotify_DesPawn_End ( )
{
	if ( !Me ) { return; }

	Me->SetOnDesPawn();

}

void UCWolfAnimInstance::AnimNotify_Att_Bite_Start ( )
{
	if ( !Me ) { return; }

	Me->IsOnBiteAtt = true;
}

void UCWolfAnimInstance::AnimNotify_Att_Special_Start ( )
{
	if ( !Me ) { return; }

	Me->IsOnBiteAtt = true;
	Me->IsOnSpecialAtt = true;
}

void UCWolfAnimInstance::AnimNotify_Land_End ( )
{
	FSM->EndAttackProcess ( );
}


void UCWolfAnimInstance::AnimNotify_Att_End ( )
{
	if ( !FSM ) { return; }
	// FSM->EndAttackProcess();

	switch ( AAttState )
	{
	case EAttackState::Attack1:
		Attack1();
		break;

	case EAttackState::Special:
		AttackSpecial();
		break;

	default:
		break;
	}

}

// FSM에서 하는게 나을지도?
void UCWolfAnimInstance::Attack1 ( )
{
	FSM->UpdateState ( EAttackState::None );
	FSM->UpdateState ( EUpperState::Jump );
	FSM->UpdateState ( EJumpState::BackJump );
	IsJumping = true;
}

void UCWolfAnimInstance::AttackSpecial ( )
{
	FSM->EndAttackProcess ( );

	// FSM->UpdateState ( EAttackState::None );
	// FSM->UpdateState (EUpperState::Idle);	// 임시값
	IsJumping = false;						// 임시2
	// FSM->TurnToTarget(FSM->TargetEnemy);
}
