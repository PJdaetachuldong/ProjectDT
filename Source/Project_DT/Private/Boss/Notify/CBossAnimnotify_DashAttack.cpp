// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimnotify_DashAttack.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "Boss/FSM/CBossFSM.h"

FString UCBossAnimnotify_DashAttack::GetNotifyName_Implementation ( ) const
{
	return "DashAttackReady";
}

void UCBossAnimnotify_DashAttack::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );
	
	My->ReadyDashAttack();
}
