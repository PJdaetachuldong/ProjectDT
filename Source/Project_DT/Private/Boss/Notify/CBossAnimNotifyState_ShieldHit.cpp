// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotifyState_ShieldHit.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotifyState_ShieldHit::GetNotifyName_Implementation ( ) const
{
	return "Reset";
}

void UCBossAnimNotifyState_ShieldHit::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	My->ShieldHitCount = 0;
}
