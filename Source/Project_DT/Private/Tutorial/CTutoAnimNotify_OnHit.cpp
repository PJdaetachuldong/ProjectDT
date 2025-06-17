// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoAnimNotify_OnHit.h"
#include "Global.h"
#include "Tutorial/CTutorialEnemy.h"

FString UCTutoAnimNotify_OnHit::GetNotifyName_Implementation() const
{
	return "OnHit";
}

void UCTutoAnimNotify_OnHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACTutorialEnemy* My = Cast<ACTutorialEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->IsDontHit = false;
}
