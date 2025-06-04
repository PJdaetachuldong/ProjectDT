// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_Guard.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_Guard::GetNotifyName_Implementation ( ) const
{
	return "CheckHit";
}

void UCBossAnimNotify_Guard::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	//가드 체크확인 콜리전 활성화
	My->OnGuardCollision();
}

void UCBossAnimNotify_Guard::NotifyTick ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float FrameDeltaTime )
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

// 	CurTime += FrameDeltaTime;
// 	if ( CurTime >= DelayTime )
// 	{
		//가드 체크확인 콜리전 활성화로 계속 체크
		My->OnGuardCollision ( );
		/*CurTime = 0.0f;*/
	/*}*/
}

void UCBossAnimNotify_Guard::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	//가드 체크확인 콜리전 비활성화
	My->OffGuardCollision();

	//가드 조건 초기화
	My->GuardGage = 0.0f;
}
