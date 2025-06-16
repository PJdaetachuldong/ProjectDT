// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoAnimNotify_ATKAfter.h"
#include "Global.h"
#include "Tutorial/CTutorialEnemy.h"

FString UCTutoAnimNotify_ATKAfter::GetNotifyName_Implementation() const
{
	return "ATKEnd";
}

void UCTutoAnimNotify_ATKAfter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACTutorialEnemy* My = Cast<ACTutorialEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->ATKEnd();
}

