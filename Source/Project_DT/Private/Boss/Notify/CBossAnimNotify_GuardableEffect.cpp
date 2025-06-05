// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_GuardableEffect.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_GuardableEffect::GetNotifyName_Implementation() const
{
	return "EnabledEffect";
}

void UCBossAnimNotify_GuardableEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);


	//현재 공격이 가드가 가능한 공격인지 체크를 함
	//현재 진행 중인 몽타주를 알아냄
	UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
	if (!NowMontage)
	{
		return;
	}

	//현재 진행중인 몽타주의 섹션을 알아냄
	FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

	//현재 진행중인 몽타주와 섹션을 함수의 매개변수로 보내서 그에 할당된 Rate값을 받아옴
	float RateScale = My->SetRateDown(NowMontage, NowSection);


	//가드 여부에 따라 소환하는 이펙트가 달라짐
	if (My->SetGuardBool(NowMontage, NowSection))
	{
		//가드 가능 이펙트 소환
	}

	else
	{
		//가드 불가능 이펙트 소환
	}
}