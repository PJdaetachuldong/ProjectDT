// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_DashEnd.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_DashEnd::GetNotifyName_Implementation ( ) const
{
	return "AttackEnd";
}

void UCBossAnimNotify_DashEnd::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify(MeshComp, Animation);

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	My->DashAttackEnd();
}
