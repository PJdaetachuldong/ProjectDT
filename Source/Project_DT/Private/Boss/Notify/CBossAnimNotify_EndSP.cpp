// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_EndSP.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "Boss/FSM/CBossFSM.h"

FString UCBossAnimNotify_EndSP::GetNotifyName_Implementation ( ) const
{
	return "ReadySet";
}

void UCBossAnimNotify_EndSP::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );
	
	My->IsReadySPAttack = false;
	My->FSMComponent->OnSpDamageAmount = 0.0f;
}
