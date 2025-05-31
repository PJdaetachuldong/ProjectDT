// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_RateDown.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_RateDown::GetNotifyName_Implementation() const
{
	return "RateDown";
}

void UCBossAnimNotify_RateDown::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	//현재 진행중인 몽타주와 섹션을 함수의 매개변수로 보내서 그에 할당된 Rate값을 받아옴
	float RateScale = My->SetRateDown(NowMontage, NowSection);

	//받은 Rate값을 설정해줌
	My->AnimInstance->Montage_SetPlayRate(NowMontage, RateScale);

	//먼저 필살기 준비 자세를 취함
	GEngine->AddOnScreenDebugMessage(170, 8.0f, FColor::Orange, TEXT("Rate Down"));
}
