// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotifyState_ShieldVFX.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotifyState_ShieldVFX::GetNotifyName_Implementation() const
{
	return "GuardVFX";
}

void UCBossAnimNotifyState_ShieldVFX::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->SpawnGuardVFX();
}

void UCBossAnimNotifyState_ShieldVFX::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);
}

void UCBossAnimNotifyState_ShieldVFX::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->DeSpawnGuardVFX();
}
