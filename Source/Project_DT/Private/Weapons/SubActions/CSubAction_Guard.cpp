// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Guard.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Weapons/CAttachment.h"
#include "Component/CMovementComponent.h"
#include "Weapons/CDoAction.h"
void UCSubAction_Guard::Pressed ( )
{
	//CheckFalse ( State->IsIdleMode ( ) );
	CheckTrue ( State->IsSubActionMode ( ) );

	Super::Pressed();

	State->SetActionMode ( );
	State->OnSubActionMode ( );

	ActionData[0].DoAction ( Owner );
	Movement->Stop ( );
}

void UCSubAction_Guard::Released ( )
{
	Super::Released ( );
	CLog::Log ( "Released" );
	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage ( );
	if ( CurrentMontage ){
		AnimInstance->Montage_JumpToSection ( FName ( "Off" ) , CurrentMontage );
	}
}

void UCSubAction_Guard::Parry ( EParryState ParryState )
{
	//CheckTrue ( State->IsSubActionMode ( ) );
	Super::Parry (ParryState );

	State->SetActionMode ( );
	State->OnSubActionMode ( );
	UE_LOG ( LogTemp , Warning , TEXT ( "Parry State: %s" ) , *UEnum::GetValueAsString ( ParryState ) );

	switch ( ParryState )
	{
	case EParryState::TL:ActionData[1].DoAction ( Owner );
		break;
	case EParryState::BL:ActionData[2].DoAction ( Owner );
		break;
	case EParryState::TR:ActionData[3].DoAction ( Owner );
		break;
	case EParryState::BR:ActionData[4].DoAction ( Owner );
		break;
	case EParryState::Max:
		break;
	default:
		break;
	}
	Movement->Stop ( );
}

void UCSubAction_Guard::Begin_SubAction_Implementation ( )
{
	Super::Begin_SubAction_Implementation ( );
}

void UCSubAction_Guard::End_SubAction_Implementation ( )
{
	Super::End_SubAction_Implementation ( );

	State->SetIdleMode ( );
	State->OffSubActionMode ( );

	Movement->Move ( );
	Movement->DisableFixedCamera ( );


	HitIndex = 0;
}

