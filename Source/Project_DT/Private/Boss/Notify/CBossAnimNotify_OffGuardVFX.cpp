// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_OffGuardVFX.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_OffGuardVFX::GetNotifyName_Implementation() const
{
	return "OffVFX";
}

void UCBossAnimNotify_OffGuardVFX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->SpawnBarrier)
	{
		My->DeSpawnGuardVFX();
	}
}
