// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Engine/DamageEvents.h"
void UCDoAction_Combo::DoAction ()
{
	CheckTrue ( DoActionDatas.Num ( ) < 1 );

	if ( bEnable )
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse ( State->IsIdleMode ( ) );

	Super::DoAction ();
	DoActionDatas[Index].DoAction ( OwnerCharacter );
}

void UCDoAction_Combo::Begin_DoAction ( )
{
	Super::Begin_DoAction ( );
	CheckFalse ( bExist );

	bExist = false;
	if(!isHeavyAttack ){
		DoActionDatas[++Index].DoAction ( OwnerCharacter );
	}
	else{
		DoHeavyActionDatas[++Index].DoHeavyAction ( OwnerCharacter );

	}
}

void UCDoAction_Combo::End_DoAction ( )
{
	Super::End_DoAction ( );

	Index = 0;
	HeavyIndex = 0;
	DamageIndex = 0;
}

void UCDoAction_Combo::DoHeavyAction ( )
{
	CheckTrue ( DoHeavyActionDatas.Num ( ) < 1 );

	if ( bEnable )
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse ( State->IsIdleMode ( ) );

	Super::DoHeavyAction ( );
	DoHeavyActionDatas[HeavyIndex].DoHeavyAction ( OwnerCharacter );

}

void UCDoAction_Combo::OnAttachmentBeginOverlap ( class ACharacter* InAttacker , AActor* InAttackCuaser , class ACharacter* InOther )
{
	Super::OnAttachmentBeginOverlap ( InAttacker , InAttackCuaser , InOther );
	CheckNull ( InOther );
	CLog::Log ( InOther->GetName ( ) );
	HitDatas[DamageIndex].SendDamage ( InAttacker , InAttackCuaser , InOther );
}

void UCDoAction_Combo::OnAttachmentEndCollision ( )
{

}
