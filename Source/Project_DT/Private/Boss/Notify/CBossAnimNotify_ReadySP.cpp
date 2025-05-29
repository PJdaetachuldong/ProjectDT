// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_ReadySP.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"


FString UCBossAnimNotify_ReadySP::GetNotifyName_Implementation ( ) const
{
	return "ReadySP";
}

void UCBossAnimNotify_ReadySP::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify(MeshComp, Animation);

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	My->IsReadySPAttack = true;
}
