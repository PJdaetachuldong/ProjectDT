// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_Backstep.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_Backstep::GetNotifyName_Implementation() const
{
	return "BackstepCheck";
}

void UCBossAnimNotify_Backstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->BackstepUse())
	{
		//현재 재생중인 애니메이션 몽타주 확인
		UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
		if (!NowMontage)
		{
			return;
		}

		//현재 몽타주가 재생중인 섹션 확인
		FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

		My->AnimInstance->Montage_JumpToSection(FName("Backstep"), NowMontage);
	}

	else
	{

	}
}
