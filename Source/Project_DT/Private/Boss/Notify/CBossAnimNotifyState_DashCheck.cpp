// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotifyState_DashCheck.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotifyState_DashCheck::GetNotifyName_Implementation ( ) const
{
	return "CheckDist";
}

void UCBossAnimNotifyState_DashCheck::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	My->RunCheckPlayerDist();
}

void UCBossAnimNotifyState_DashCheck::NotifyTick ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float FrameDeltaTime )
{
	Super::NotifyTick ( MeshComp , Animation , FrameDeltaTime );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	My->RunCheckPlayerDist();
}

void UCBossAnimNotifyState_DashCheck::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );
}
