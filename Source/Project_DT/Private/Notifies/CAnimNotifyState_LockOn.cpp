// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_LockOn.h"
#include "Global.h"
#include "Component/CTargetingComponent.h"
FString UCAnimNotifyState_LockOn::GetNotifyName_Implementation ( ) const
{
	return "LockOn";
}

void UCAnimNotifyState_LockOn::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCTargetingComponent* Target = CHelpers::GetComponent<UCTargetingComponent> ( MeshComp->GetOwner ( ) );
	CheckNull ( Target );
	Target->bSearchTarget = true;
	Target->OnLookOn ( );
}

void UCAnimNotifyState_LockOn::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	UCTargetingComponent* Target = CHelpers::GetComponent<UCTargetingComponent> ( MeshComp->GetOwner ( ) );
	CheckNull ( Target );
	Target->bSearchTarget = false;

}

