// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_CounterAction.h"
#include "Weapons/CWeaponComponent.h"
#include "Global.h"
#include "Weapons/CDoAction.h"

FString UCAnimNotify_CounterAction::GetNotifyName_Implementation ( ) const
{
	return "Begin_DoAction";
}

void UCAnimNotify_CounterAction::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify ( MeshComp , Animation );
	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent> ( MeshComp->GetOwner ( ) );
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent> ( MeshComp->GetOwner ( ) );
	CheckNull ( weapon );
	CheckNull ( State );
	CheckNull ( weapon->GetDoAction ( ) );

	weapon->GetDoAction ( )->CounterAction ( );
    State->SetCounterMode();
	
}
