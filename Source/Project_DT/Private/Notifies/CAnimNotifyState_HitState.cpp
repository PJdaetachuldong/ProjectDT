// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_HitState.h"
#include "Global.h"

FString UCAnimNotifyState_HitState::GetNotifyName_Implementation ( ) const
{
	return "HitState";
}

void UCAnimNotifyState_HitState::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );
	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCStateComponent* State=CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(State);
	State->SetHittedMode();
}

void UCAnimNotifyState_HitState::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCStateComponent* State=CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(State);
	State->SetIdleMode();
}