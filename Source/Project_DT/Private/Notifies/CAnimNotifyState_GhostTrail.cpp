// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_GhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
FString UCAnimNotifyState_GhostTrail::GetNotifyName_Implementation() const
{
	return "GhostTrail";
}

void UCAnimNotifyState_GhostTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(GhostTrailClass);
	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);

}

void UCAnimNotifyState_GhostTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(GhostTrail);
	GhostTrail->Destroy();
}
