// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "Component/CStateComponent.h"
void UCDoAction_Combo::DoAction ( )
{
	CheckTrue ( DoActionDatas.Num ( ) < 1 );

	if ( bEnable )
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse ( State->IsIdleMode ( ) );

	Super::DoAction ( );
	DoActionDatas[Index].DoAction ( OwnerCharacter );
}

void UCDoAction_Combo::Begin_DoAction ( )
{
	Super::Begin_DoAction ( );
	CheckFalse ( bExist );

	bExist = false;
	DoActionDatas[++Index].DoAction ( OwnerCharacter );
}

void UCDoAction_Combo::End_DoAction ( )
{
	Super::End_DoAction ( );

	Index = 0;
}