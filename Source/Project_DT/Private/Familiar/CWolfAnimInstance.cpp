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