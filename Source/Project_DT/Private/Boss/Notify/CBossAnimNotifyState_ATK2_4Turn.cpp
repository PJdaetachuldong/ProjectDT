// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotifyState_ATK2_4Turn.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotifyState_ATK2_4Turn::GetNotifyName_Implementation() const
{
	return "move";
}

void UCBossAnimNotifyState_ATK2_4Turn::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);
}

void UCBossAnimNotifyState_ATK2_4Turn::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->ATK2_4Turn();
}

void UCBossAnimNotifyState_ATK2_4Turn::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);
}
