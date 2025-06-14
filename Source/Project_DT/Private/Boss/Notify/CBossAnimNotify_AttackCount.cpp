// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_AttackCount.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "Boss/CBossWeapon.h"
#include "Boss/CBossAnim.h"

FString UCBossAnimNotify_AttackCount::GetNotifyName_Implementation() const
{
	return "AddNumber";
}

void UCBossAnimNotify_AttackCount::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	//원거리, 돌진, 가드->카운터, 필살기이면 따로 실행
	if (NowMontage == My->AM_RangedAttack || NowMontage == My->AM_DashAttack || NowMontage == My->AM_Guard || NowMontage == My->AM_SPAttack)
	{
		if (NowMontage == My->AM_RangedAttack)
		{
			My->SpawnWeapon->HitNumber = 8;
		}
		else if (NowMontage == My->AM_DashAttack)
		{
			My->SpawnWeapon->HitNumber = 9;
		}
		else if (NowMontage == My->AM_Guard)
		{
			My->SpawnWeapon->HitNumber = 10;
		}
		else if (NowMontage == My->AM_SPAttack)
		{
			if (FName("SPAttackLoop") == My->AnimInstance->Montage_GetCurrentSection(NowMontage))
			{
				My->SpawnWeapon->HitNumber = 11;
			}

			else if (FName("SPAttack") == My->AnimInstance->Montage_GetCurrentSection(NowMontage))
			{
				My->SpawnWeapon->HitNumber = 12;
			}
		}
	}

	else
	{ 
		//현재 진행중인 몽타주의 섹션을 알아냄
		FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

		EAttackType ComboType = My->AttackSectionToEnumMap.FindRef(NowSection, EAttackType::None);

		switch (ComboType)
		{
		case EAttackType::Attack01_01: My->SpawnWeapon->HitNumber = 0; break;
		case EAttackType::Attack01_02: My->SpawnWeapon->HitNumber = 1; break;
		case EAttackType::Attack01_03: My->SpawnWeapon->HitNumber = 2; break;
		case EAttackType::Attack01_04: My->SpawnWeapon->HitNumber = 3; break;
		case EAttackType::Attack02_01: My->SpawnWeapon->HitNumber = 4; break;
		case EAttackType::Attack02_02: My->SpawnWeapon->HitNumber = 5; break;
		case EAttackType::Attack02_03: My->SpawnWeapon->HitNumber = 6; break;
		case EAttackType::Attack02_04: My->SpawnWeapon->HitNumber = 7; break;
		default: My->SpawnWeapon->HitNumber = -1; break;
		}
	}
}
