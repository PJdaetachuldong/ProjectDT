// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_TimeManager.h"
#include "Global.h"
#include "GameFramework/Character.h"
FString UCAnimNotifyState_TimeManager::GetNotifyName_Implementation() const
{
	return "TimeManager";
}

void UCAnimNotifyState_TimeManager::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	// 글로벌 타임 느리게 (10% 속도)
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), DelayTime);
}

void UCAnimNotifyState_TimeManager::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	// 글로벌 타임 원래대로
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 1.0f);
}