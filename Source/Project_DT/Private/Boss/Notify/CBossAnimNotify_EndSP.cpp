// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_EndSP.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_EndSP::GetNotifyName_Implementation ( ) const
{
	return "EndSP";
}

void UCBossAnimNotify_EndSP::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	My->IsReadySPAttack = false;
	My->OnSPDamage = 0.0f;
}
