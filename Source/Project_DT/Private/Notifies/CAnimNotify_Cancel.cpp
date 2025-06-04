// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_Cancel.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Weapons/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

FString UCAnimNotify_Cancel::GetNotifyName_Implementation() const
{
	return "Cancel";
}

void UCAnimNotify_Cancel::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(State);
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	weapon->GetDoAction()->ResetDoAction();

	State->SetCancelMode();
}
