// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_RateReturn.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_RateReturn::GetNotifyName_Implementation() const
{
	return "RateDown";
}

void UCBossAnimNotify_RateReturn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	//현재 진행 중인 몽타주를 알아냄
	UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
	if (!NowMontage)
	{
		return;
	}

	//현재 진행중인 몽타주의 섹션을 알아냄
	FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

	//받은 Rate값을 설정해줌
	My->AnimInstance->Montage_SetPlayRate(NowMontage, 1.0f);
}