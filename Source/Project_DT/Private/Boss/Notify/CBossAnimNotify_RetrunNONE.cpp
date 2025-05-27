// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_RetrunNONE.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "Boss/FSM/CBossFSM.h"

FString UCBossAnimNotify_RetrunNONE::GetNotifyName_Implementation ( ) const
{
	return "NONE";
}

void UCBossAnimNotify_RetrunNONE::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );
	
	My->FSMComponent->AttackState = EBossATTACKState::NONE;
}
