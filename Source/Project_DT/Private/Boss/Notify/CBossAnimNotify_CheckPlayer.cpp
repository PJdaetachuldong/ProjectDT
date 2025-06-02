// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_CheckPlayer.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "Animation/AnimInstance.h"

FString UCBossAnimNotify_CheckPlayer::GetNotifyName_Implementation ( ) const
{
	return "CheckPlayer";
}

void UCBossAnimNotify_CheckPlayer::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	//현재 재생중인 애니메이션 몽타주 확인
	UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
	if ( !NowMontage )
	{
		return;
	}

	//현재 몽타주가 재생중인 섹션 확인
	FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);
	
	//플레이어와의 거리와 시각을 판단해서 공격이 가능한지 체크
	bool CanNextAttack = My->CheckPlayer();

	if ( CanNextAttack )
	{
		//다음 섹션 공격 애니메이션이 실행되도록 만듦
		My->PlayNextSectionAttack(NowMontage, NowSection);
	}

	else
	{
		//그대로 끝 공격
	}
}

void UCBossAnimNotify_CheckPlayer::NotifyTick ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float FrameDeltaTime )
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );
	
	//현재 재생중인 애니메이션 몽타주 확인
	UAnimMontage* NowMontage = MeshComp->GetAnimInstance ( )->GetCurrentActiveMontage ( );
	if ( !NowMontage )
	{
		return;
	}

	//현재 몽타주가 재생중인 섹션 확인
	FName NowSection = MeshComp->GetAnimInstance ( )->Montage_GetCurrentSection ( NowMontage );

	CurTime += FrameDeltaTime;

	if ( CurTime >= DelayTime )
	{
		//플레이어와의 거리와 시각을 판단해서 공격이 가능한지 체크
		bool CanNextAttack = My->CheckPlayer ( );

		if ( CanNextAttack )
		{
			//다음 섹션 공격 애니메이션이 실행되도록 만듦
			My->PlayNextSectionAttack ( NowMontage , NowSection );
		}

		else
		{
			//그대로 끝 공격
		}
	}
}

void UCBossAnimNotify_CheckPlayer::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );
}
