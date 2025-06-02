// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossNotifyState_Collision.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossNotifyState_Collision::GetNotifyName_Implementation ( ) const
{
	return "Collision";
}

void UCBossNotifyState_Collision::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull ( My );

	//무기 콜리전 활성화
	My->OnSwordCollision();
}

void UCBossNotifyState_Collision::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation );

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull ( My );

	//무기 콜리전 비활성화
	My->OffSwordCollision();
}
