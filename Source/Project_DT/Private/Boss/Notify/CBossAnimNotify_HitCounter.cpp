// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_HitCounter.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"


FString UCBossAnimNotify_HitCounter::GetNotifyName_Implementation() const
{
	return "HitCounter";
}

void UCBossAnimNotify_HitCounter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->ShieldHitCount = 0;
}
