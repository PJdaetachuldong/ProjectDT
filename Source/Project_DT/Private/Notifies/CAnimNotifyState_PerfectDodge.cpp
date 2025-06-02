// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_PerfectDodge.h"
#include "Global.h"
#include "Component/CPerfectDodgeComponent.h"
#include "Component/CTargetingComponent.h"
FString UCAnimNotifyState_PerfectDodge::GetNotifyName_Implementation() const
{
	return "PerfectDodge";
}

void UCAnimNotifyState_PerfectDodge::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCPerfectDodgeComponent* Dodge = CHelpers::GetComponent<UCPerfectDodgeComponent>(MeshComp->GetOwner());
	UCTargetingComponent* Target = CHelpers::GetComponent<UCTargetingComponent>(MeshComp->GetOwner());
	CheckNull(Dodge);
	CheckNull(Target);
	//Target->ResetLockOn();
	Dodge->StartPerfectDodge();

}

void UCAnimNotifyState_PerfectDodge::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCPerfectDodgeComponent* Dodge = CHelpers::GetComponent<UCPerfectDodgeComponent>(MeshComp->GetOwner());
	CheckNull(Dodge);

	Dodge->EndPerfectDodge();
}