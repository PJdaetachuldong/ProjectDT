// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_CameraAction.h"

#include "Component/CCameraActionComponent.h"
#include "Utilities/CHelper.h"

FString UCAnimNotifyState_CameraAction::GetNotifyName_Implementation() const
{
	return "CameraAction";

}

void UCAnimNotifyState_CameraAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner());

	UCCameraActionComponent* Camera= CHelpers::GetComponent<UCCameraActionComponent>(MeshComp->GetOwner());
	CheckNull ( Camera);
	
	Camera->CameraActionStart(ArmVector,ArmRotation,SpringArmLength,Second);
	
}

void UCAnimNotifyState_CameraAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner());
	
	UCCameraActionComponent* Camera= CHelpers::GetComponent<UCCameraActionComponent>(MeshComp->GetOwner());
	CheckNull ( Camera);
	Camera->CameraActionEnd();
	
}
