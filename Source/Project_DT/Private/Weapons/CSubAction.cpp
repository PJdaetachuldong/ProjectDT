// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CSubAction.h"
#include "Weapons/CAttachment.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
#include "Weapons/CWeaponComponent.h"

void UCSubAction::BeginPlay ( class ACharacter* InOwner , class ACAttachment* InAttachment , class UCDoAction* InDoAction )
{
	Owner = InOwner;
	Attachment = InAttachment;
	AnimInstance = Owner->GetMesh ( )->GetAnimInstance ( );

	State = CHelpers::GetComponent<UCStateComponent> ( InOwner );
	Movement = CHelpers::GetComponent<UCMovementComponent> ( InOwner );
	Weapon = CHelpers::GetComponent<UCWeaponComponent> ( InOwner );
	DoAction=InDoAction;
}