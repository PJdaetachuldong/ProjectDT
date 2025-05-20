// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoActionGuard.h"
#include "Global.h"
#include "Component/CStateComponent.h"
void UCDoActionGuard::DoGuardStarted ( )
{
	//CheckTrue ( DoGuardActionData.Num ( ) < 1 );

	//if ( bEnable )
	//{
	//	bEnable = false;
	//	bExist = true;

	//	return;
	//}

	//CheckFalse ( State->IsIdleMode ( ) );
	Super::DoGuardStarted ( );
	DoGuardActionData[0].DoGuardAction ( OwnerCharacter );
}

void UCDoActionGuard::DoGuardTrriger ( )
{

}

void UCDoActionGuard::DoGuardComplete ( )
{
	CheckTrue ( DoGuardActionData.Num ( ) < 1 );

	//if ( bEnable )
	//{
	//	bEnable = false;
	//	bExist = true;

	//	return;
	//}

	CheckFalse ( State->IsIdleMode ( ) );

	Super::DoGuardComplete ( );
	DoGuardActionData[1].DoGuardAction ( OwnerCharacter );
}
