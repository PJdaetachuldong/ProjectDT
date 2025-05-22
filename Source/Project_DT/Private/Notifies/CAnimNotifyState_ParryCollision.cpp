// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_ParryCollision.h"
#include "Global.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Component/CParryComponent.h"
FString UCAnimNotifyState_ParryCollision::GetNotifyName_Implementation ( ) const
{
	return "Parry";
}

void UCAnimNotifyState_ParryCollision::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );
	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCParryComponent* parry = CHelpers::GetComponent<UCParryComponent> ( MeshComp->GetOwner ( ) );
	CheckNull ( parry );

	parry->OnParryCollision();
}

void UCAnimNotifyState_ParryCollision::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCParryComponent* parry = CHelpers::GetComponent<UCParryComponent> ( MeshComp->GetOwner ( ) );
	CheckNull ( parry );

	parry->OffParryCollision ( );
}

