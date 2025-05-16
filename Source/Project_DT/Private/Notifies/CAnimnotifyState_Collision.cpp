// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimnotifyState_Collision.h"
#include "Global.h"
#include "Weapons/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CAttachment.h"


FString UCAnimnotifyState_Collision::GetNotifyName_Implementation ( ) const
{
	return "Collision";
}

void UCAnimnotifyState_Collision::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );
	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent> ( MeshComp->GetOwner ( ) );
	CheckNull ( weapon );
	CheckNull ( weapon->GetAttachment ( ) );

	weapon->GetAttachment ( )->OnCollisions ( );
}

void UCAnimnotifyState_Collision::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent> ( MeshComp->GetOwner ( ) );
	CheckNull ( weapon );
	CheckNull ( weapon->GetAttachment ( ) );

	weapon->GetAttachment ( )->OffCollisions ( );
}