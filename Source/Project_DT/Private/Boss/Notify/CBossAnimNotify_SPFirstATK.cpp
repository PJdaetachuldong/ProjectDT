// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_SPFirstATK.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "Boss/FSM/CBossFSM.h"
#include "Boss/CBossAnim.h"

FString UCBossAnimNotify_SPFirstATK::GetNotifyName_Implementation() const
{
	return "HitCheck";
}

void UCBossAnimNotify_SPFirstATK::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->IsSPFirstATKHit)
	{
		//현재 재생중인 애니메이션 몽타주 확인
		UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
		if (!NowMontage)
		{
			return;
		}

		//현재 몽타주가 재생중인 섹션 확인
		FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

		My->AnimInstance->Montage_JumpToSection(FName("SPAttackStart"), NowMontage);
	}
}

void UCBossAnimNotify_SPFirstATK::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->IsSPFirstATKHit)
	{
		//현재 재생중인 애니메이션 몽타주 확인
		UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
		if (!NowMontage)
		{
			return;
		}

		//현재 몽타주가 재생중인 섹션 확인
		FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

		My->AnimInstance->Montage_JumpToSection(FName("SPAttackStart"), NowMontage);
	}
}

void UCBossAnimNotify_SPFirstATK::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->IsSPFirstATKHit)
	{
		//현재 재생중인 애니메이션 몽타주 확인
		UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
		if (!NowMontage)
		{
			return;
		}

		//현재 몽타주가 재생중인 섹션 확인
		FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

		My->AnimInstance->Montage_JumpToSection(FName("SPAttackStart"), NowMontage);
	}

	else
	{
		My->FSMComponent->State = EBossState::ATTACK;
		My->FSMComponent->AttackState = EBossATTACKState::NONE;
	}
}
