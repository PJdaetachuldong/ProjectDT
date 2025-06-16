// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoAnimNotify_ATKTurn.h"
#include "Global.h"
#include "Tutorial/CTutorialEnemy.h"

FString UCTutoAnimNotify_ATKTurn::GetNotifyName_Implementation() const
{
	return "ATKTurn";
}

void UCTutoAnimNotify_ATKTurn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACTutorialEnemy* My = Cast<ACTutorialEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->AttackTurn();
}
