// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Skill.h"
#include "Weapons/CAttachment.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Component/CMovementComponent.h"
void UCSubAction_Skill::BeginPlay ( class ACharacter* InOwner , class ACAttachment* InAttachment , class UCDoAction* InDoAction )
{
	Owner = InOwner;
	Attachment = InAttachment;

	State = CHelpers::GetComponent<UCStateComponent> ( InOwner );
	Movement = CHelpers::GetComponent<UCMovementComponent> ( InOwner );
}

void UCSubAction_Skill::Pressed ( )
{
	State->OnSubActionMode ( );
}
