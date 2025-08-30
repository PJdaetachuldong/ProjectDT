// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_HealState.h"
#include "Global.h"
#include "Component/CStatusComponent.h"

FString UCAnimNotifyState_HealState::GetNotifyName_Implementation ( ) const
{
	return "HitState";
}

void UCAnimNotifyState_HealState::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );
	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCStateComponent* State=CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(State);
	State->SetHealMode();
}

void UCAnimNotifyState_HealState::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCStateComponent* State=CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	UCStatusComponent* Status=CHelpers::GetComponent<UCStatusComponent>(MeshComp->GetOwner());
	CheckNull(State);
	Status->Heal(50);
	Status->UseMana(40);
	State->SetIdleMode();
}