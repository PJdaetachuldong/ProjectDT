// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Guard.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Weapons/CAttachment.h"
#include "Component/CMovementComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponComponent.h"

void UCSubAction_Guard::Pressed ( )
{
	CheckTrue ( State->IsHittedMode());
	CheckTrue ( State->IsSubActionMode ( ) );
	CheckTrue ( State->IsGuardMode());
	Super::Pressed();

	State->SetActionMode ( );
	State->OnSubActionMode ( );

	ActionData[0].DoAction ( Owner );
	Movement->Stop ( );
}

void UCSubAction_Guard::Released ( )
{
	Super::Released ( );
	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage ( );
	CheckFalse( State->IsGuardMode());
	if (CurrentMontage)
		ActionData[1].DoAction ( Owner );

}

void UCSubAction_Guard::Begin_SubAction_Implementation ( )
{
	Super::Begin_SubAction_Implementation ( );
	State->OnSubActionMode ( );
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

