// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotifyState_Backsteping.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotifyState_Backsteping::GetNotifyName_Implementation() const
{
	return "Backsteping";
}

void UCBossAnimNotifyState_Backsteping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	//현재 에너미와 플레이어 간에 방향을 구함
	FVector PlayerLocation = My->Target->GetActorLocation();
	FVector PawnLocation = My->GetActorLocation();
	ToPlayerDirection = (PlayerLocation - PawnLocation).GetSafeNormal();
}

void UCBossAnimNotifyState_Backsteping::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	//구한 전방 방향을 사용해서 뒤로 해당 구간동안 뒤로 물러나게 만듦
	My->SetActorLocation(My->GetActorLocation() + -ToPlayerDirection * 500.0f * FrameDeltaTime);
}

void UCBossAnimNotifyState_Backsteping::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	//값을 초기화함
}

