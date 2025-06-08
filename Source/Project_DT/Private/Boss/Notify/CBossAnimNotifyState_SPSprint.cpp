// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotifyState_SPSprint.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UCBossAnimNotifyState_SPSprint::GetNotifyName_Implementation() const
{
	return "Run";
}

void UCBossAnimNotifyState_SPSprint::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->IsSPRun = true;
	
	My->bUseControllerRotationYaw = false; // AI 컨트롤러 회전 비활성화
	My->GetCharacterMovement()->bOrientRotationToMovement = false; //
}

void UCBossAnimNotifyState_SPSprint::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	My->IsSPRun = true;
}

void UCBossAnimNotifyState_SPSprint::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);
}
