// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoAnimNotify_IDLEEnd.h"
#include "Global.h"
#include "Tutorial/CTutorialEnemy.h"

FString UCTutoAnimNotify_IDLEEnd::GetNotifyName_Implementation() const
{
	return "IDLEEnd";
}

void UCTutoAnimNotify_IDLEEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACTutorialEnemy* My = Cast<ACTutorialEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->IDLEEnd();
}
