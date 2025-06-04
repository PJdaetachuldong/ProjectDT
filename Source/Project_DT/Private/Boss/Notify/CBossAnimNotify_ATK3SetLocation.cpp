// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_ATK3SetLocation.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_ATK3SetLocation::GetNotifyName_Implementation() const
{
	return "SetLocation";
}

void UCBossAnimNotify_ATK3SetLocation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->SetLocation();
}
