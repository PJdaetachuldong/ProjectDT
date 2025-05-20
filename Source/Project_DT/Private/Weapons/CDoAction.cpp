// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CDoAction.h"
#include "Utilities/CHelper.h"
#include "Component/CMovementComponent.h"
#include "Component/CStateComponent.h"
#include "Global.h"
UCDoAction::UCDoAction ( )
{

}

void UCDoAction::BeginPlay ( class ACAttachment* InAttachment , class UCEquipment* InEquipment , class ACharacter* InOwner , class TArray<FDoActionData>& InDoActionData , class TArray<FDoHeavyActionData>& InDoHeavyActionData , class TArray<FDoSpecialActionData>& InDoSpecialActionData , class TArray<FDoGuardActionData>& InDoGuardActionData , TArray<FHitData>& InHitData )
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld ( );

	State = CHelpers::GetComponent<UCStateComponent> ( InOwner );
	Movement = CHelpers::GetComponent<UCMovementComponent> ( InOwner );

	DoActionDatas = InDoActionData;
	DoHeavyActionDatas = InDoHeavyActionData;
	DoSpecialActionData = InDoSpecialActionData;
	DoGuardActionData = InDoGuardActionData;
	HitDatas = InHitData;
}

void UCDoAction::DoAction ( )
{
	State->SetActionMode ();
}
void UCDoAction::Begin_DoAction ( )
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction ( )
{
	bBeginAction = false;

	State->SetIdleMode ( );
	Movement->Move ( );
	Movement->DisableFixedCamera ( );

}

void UCDoAction::DoHeavyAction ( )
{
	State->SetActionMode ( );
}

void UCDoAction::Begin_DoHeavyAction ( )
{
	bBeginAction = true;
}

void UCDoAction::DoSpecialAction ( )
{
	State->SetActionMode ( );

}

void UCDoAction::DoGuardStarted ( )
{
	State->SetGuardMode ( );
	CLog::Log ( "asdjasibdihasbdsad" );

}

void UCDoAction::DoGuardComplete ( )
{
	State->SetIdleMode ( );

}
