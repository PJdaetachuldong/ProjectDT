// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_SetSPColli.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_SetSPColli::GetNotifyName_Implementation() const
{
	return "SpawnSPColli";
}

void UCBossAnimNotify_SetSPColli::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->SetSPColli();
}