// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SubActions/CSubAction_Skill1.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Weapons/CAttachment.h"
#include "Component/CMovementComponent.h"
#include "Weapons/CDoAction.h"
void UCSubAction_Skill1::Pressed ( )
{
	//CheckFalse ( State->IsIdleMode ( ) );
	CLog::Log ( "Pressed1" );
	CheckTrue ( State->IsSubActionMode ( ) );
	CLog::Log ( "Pressed2" );
	Super::Pressed ( );

	State->SetActionMode ( );
	State->OnSubActionMode ( );

	ActionData.DoAction ( Owner );
}
void UCSubAction_Skill1::Begin_SubAction_Skill_Implementation ( )
{
	Super::Begin_SubAction_Skill_Implementation ( );

	//Attachment->OnAttachmentEndCollision.Remove ( DoAction , "OnAttachmentEndCollision" );
	//Attachment->OnAttachmentBeginOverlap.Remove ( DoAction , "OnAttachmentBeginOverlap" );

	//Attachment->OnAttachmentEndCollision.AddDynamic ( this , &UCSubAction_Fist::OnAttachmentEndCollision );
	//Attachment->OnAttachmentBeginOverlap.AddDynamic ( this , &UCSubAction_Fist::OnAttachmentBeginOverlap );
}

void UCSubAction_Skill1::End_SubAction_Skill_Implementation ( )
{
	Super::End_SubAction_Skill_Implementation ( );

	//Attachment->OnAttachmentEndCollision.Remove ( this , "OnAttachmentEndCollision" );
	//Attachment->OnAttachmentBeginOverlap.Remove ( this , "OnAttachmentBeginOverlap" );

	//Attachment->OnAttachmentEndCollision.AddDynamic ( DoAction , &UCDoAction::OnAttachmentEndCollision );
	//Attachment->OnAttachmentBeginOverlap.AddDynamic ( DoAction , &UCDoAction::OnAttachmentBeginOverlap );


	State->SetIdleMode ( );
	State->OffSubActionMode ( );

	Movement->Move ( );
	Movement->DisableFixedCamera ( );


	HitIndex = 0;
}

