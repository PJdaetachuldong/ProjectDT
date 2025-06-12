// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_Guard.h"
#include "Global.h"
#include "Component/CParryComponent.h"
FString UCAnimNotifyState_Guard::GetNotifyName_Implementation() const
{
	return "GuardCollision";
}

void UCAnimNotifyState_Guard::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	UCParryComponent* Parry= CHelpers::GetComponent<UCParryComponent>(MeshComp->GetOwner());

	CheckNull(Parry);

	Parry->OnGuard();

}

void UCAnimNotifyState_Guard::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());
	UCParryComponent* Parry = CHelpers::GetComponent<UCParryComponent>(MeshComp->GetOwner());

	CheckNull(Parry);

	Parry->OffGuard();
}
