// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_SPDistCheck.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "Boss/FSM/CBossFSM.h"
#include "Boss/CBossAnim.h"

FString UCBossAnimNotify_SPDistCheck::GetNotifyName_Implementation() const
{
	return "CheckDist";
}

void UCBossAnimNotify_SPDistCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	//현재 재생중인 애니메이션 몽타주 확인
	UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
	if (!NowMontage)
	{
		return;
	}

	//현재 몽타주가 재생중인 섹션 확인
	FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);
	
	//플레이어랑 거리가 400이상면 달려간 다음 찌름
	if (My->FSMComponent->TargetDist >= 400.0f)
	{
		My->AnimInstance->Montage_JumpToSection(FName("Run"), NowMontage);
	}

	//아니면 (가까운 경우) 바로 찌르기 동작 실행
	else
	{
		My->AnimInstance->Montage_JumpToSection(FName("SPFirstATK"), NowMontage);
	}
}
