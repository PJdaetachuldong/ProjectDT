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


// ===================== 노티파이 추가하는 파트 =====================

void UCWolfAnimInstance::AnimNotify_Roar_END ( )
{
	if ( !Me ) { return; }
	FSM->UpdateState(EUpperState::Idle);
}

void UCWolfAnimInstance::AnimNotify_Search_Target ( )
{
	if ( !Me ) { return; }
	FSM->SearchEnemy ( );
	Me->IsCanAttack = true;
}

void UCWolfAnimInstance::AnimNotify_Dissolve_Start ( )
{
	if ( !Me ) { return; }

	FSM->Dissolved();
}

void UCWolfAnimInstance::AnimNotify_DesPawn_End ( )
{
	if ( !Me ) { return; }

	Me->SetOnDesPawn();

}

void UCWolfAnimInstance::AnimNotify_Att_End ( )
{
	if ( !Me ) { return; }
	FSM->EndAttackProcess();
}
