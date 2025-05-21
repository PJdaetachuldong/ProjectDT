// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_ResetAction.h"
#include "Global.h"
#include "Weapons/CWeaponComponent.h"
#include "Weapons/CDoAction.h"
FString UCAnimNotify_ResetAction::GetNotifyName_Implementation ( ) const
{
	return "End_DoAction";
}

void UCAnimNotify_ResetAction::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify ( MeshComp , Animation );
	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent> ( MeshComp->GetOwner ( ) );
	CheckNull ( weapon );
	CheckNull ( weapon->GetDoAction ( ) );

	weapon->GetDoAction ( )->ResetDoAction ( );
}
