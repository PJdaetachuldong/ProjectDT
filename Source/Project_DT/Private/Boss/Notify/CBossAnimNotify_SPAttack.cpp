// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_SPAttack.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_SPAttack::GetNotifyName_Implementation() const
{
	return "SPAttack";
}

void UCBossAnimNotify_SPAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->SPAttack();
}
