// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoAnimNotify_DIE.h"
#include "Global.h"
#include "Tutorial/CTutorialEnemy.h"

FString UCTutoAnimNotify_DIE::GetNotifyName_Implementation() const
{
	return "DIE";
}

void UCTutoAnimNotify_DIE::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACTutorialEnemy* My = Cast<ACTutorialEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->DIESetLocation();
}