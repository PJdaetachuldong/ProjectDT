// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CDoAction.h"
#include "Utilities/CHelper.h"
#include "Component/CMovementComponent.h"
#include "Component/CStateComponent.h"
#include "Global.h"
UCDoAction::UCDoAction ( )
{

}

void UCDoAction::BeginPlay (
	class ACAttachment* InAttachment ,
	class UCEquipment* InEquipment ,
	class ACharacter* InOwner ,
	class TArray<FDoActionData>& InDoActionData ,
	class TArray<FDoHeavyActionData>& InDoHeavyActionData,
	class TArray<FCounterActionData>& InCounterActionDatas,
	class TArray<FParryActionData>& InParryActionData ,
	TArray<FHitData>& InHitData )
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld ( );

	State = CHelpers::GetComponent<UCStateComponent> ( InOwner );
	Movement = CHelpers::GetComponent<UCMovementComponent> ( InOwner );

	DoActionDatas = InDoActionData;
	DoHeavyActionDatas = InDoHeavyActionData;
	CounterActionDatas = InCounterActionDatas;
	ParryActionDatas = InParryActionData;
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

void UCDoAction::DoActionParry ( EParryState parryState )
{
	State->SetActionMode ( );
}

void UCDoAction::CounterAction ( )
{
	bBeginAction = true;
}
