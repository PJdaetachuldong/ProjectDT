// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_SPEnd.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_SPEnd::GetNotifyName_Implementation() const
{
	return "SPEnd";
}

void UCBossAnimNotify_SPEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->EndSPAttack();
}
