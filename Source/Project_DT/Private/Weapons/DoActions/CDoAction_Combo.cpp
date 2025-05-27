// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Engine/DamageEvents.h"
void UCDoAction_Combo::DoAction ()
{
	CheckTrue ( DoActionDatas.Num ( ) < 1 );
	CheckTrue ( State->IsSubActionMode ( ) );

	if ( bEnable )
	{
		bEnable = false;
		bExist = true;

		return;
	}
	CheckFalse ( State->IsIdleMode ());
	if ( bParry )return;


	Super::DoAction ();
	AddComboArray(DoActionDatas[0].DoAction ( OwnerCharacter ));
	DamageIndex = 0;
}

void UCDoAction_Combo::Begin_DoAction ( )
{
	Super::Begin_DoAction ( );
	CheckFalse ( bExist );

	bExist = false;
	switch ( ActionState )
	{
	case EActionState::Normal: {
		AddComboArray ( DoActionDatas[++Index].DoAction ( OwnerCharacter ) );
		DamageIndex = Index;
	}
							 break;
	case EActionState::Heavy: {
		AddComboArray ( DoHeavyActionDatas[++HeavyIndex].DoHeavyAction ( OwnerCharacter ) );
		DamageIndex = HeavyIndex + 3;
	}
							break;
	default:
		break;
	}
}

void UCDoAction_Combo::End_DoAction ( )
{
	Super::End_DoAction ( );

	Index = 0;
	HeavyIndex = 0;
	DamageIndex = 0;
	bParry = false;

}

void UCDoAction_Combo::DoHeavyAction ( )
{
	CheckTrue ( DoHeavyActionDatas.Num ( ) < 1 );
	CheckTrue ( State->IsSubActionMode ( ) );


	if ( bEnable )
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse ( State->IsIdleMode ( ) );
	if ( bParry )return;

	Super::DoHeavyAction ( );
	AddComboArray(DoHeavyActionDatas[0].DoHeavyAction ( OwnerCharacter ));
	DamageIndex = 3;
}

void UCDoAction_Combo::DoActionParry ( EParryState parryState)
{
	CheckTrue ( ParryActionDatas.Num ( ) < 1 );

	if ( bEnable )
	{
		bEnable = false;
		bExist = true;

		return;
	}

	Super::DoActionParry ( parryState );
	switch ( parryState )
	{
	case EParryState::TL:ParryActionDatas[0].DoParryAction ( OwnerCharacter );
		break;
	case EParryState::BL:ParryActionDatas[1].DoParryAction ( OwnerCharacter );
		break;
	case EParryState::TR:ParryActionDatas[2].DoParryAction ( OwnerCharacter );
		break;
	case EParryState::BR:ParryActionDatas[3].DoParryAction ( OwnerCharacter );
		break;
	case EParryState::Max:
		break;
	default:
		break;
	}

}

void UCDoAction_Combo::Begin_Parry ( )
{
	Super::Begin_Parry ( );
	bParry = true;
}

void UCDoAction_Combo::End_Parry ( )
{
	Super::End_Parry ( );
	bParry = false;

}

void UCDoAction_Combo::CounterAction ( )
{
	CheckTrue ( CounterActionDatas.Num ( ) < 1 );

	CheckFalse ( bExist );

	bExist = false;


	Super::CounterAction ( );
	CounterActionDatas[0].DoCounterAction ( OwnerCharacter );
}

void UCDoAction_Combo::ResetDoAction ( )
{
	Index = 0;
	HeavyIndex = 0;
	DamageIndex = 0;

	//bBeginAction = false;

}

void UCDoAction_Combo::OnAttachmentBeginOverlap ( class ACharacter* InAttacker , AActor* InAttackCuaser , class ACharacter* InOther )
{
	Super::OnAttachmentBeginOverlap ( InAttacker , InAttackCuaser , InOther );
	CheckNull ( InOther );
	HitDatas[DamageIndex].SendDamage ( InAttacker , InAttackCuaser , InOther );
}

void UCDoAction_Combo::AddComboArray ( FString NewCombo )
{
	if ( ComboArray.Num ( ) >= 3 )
	{
		ComboArray.RemoveAt ( 0 );
	}

	ComboArray.Add ( NewCombo );

}

bool UCDoAction_Combo::IsLeftMajority ( )
{
	int32 LeftCount = 0;
	int32 OtherCount = 0;

	for ( const FString& Combo : ComboArray )
	{
		if ( Combo.Equals ( TEXT ( "Left" ) , ESearchCase::IgnoreCase ) )
		{
			LeftCount++;
		}
		else
		{
			OtherCount++;
		}
	}

	return LeftCount <= OtherCount;
}

