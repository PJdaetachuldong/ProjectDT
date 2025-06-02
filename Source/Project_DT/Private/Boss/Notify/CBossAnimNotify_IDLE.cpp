// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_IDLE.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"
#include "Boss/CBossAnim.h"


FString UCBossAnimNotify_IDLE::GetNotifyName_Implementation ( ) const
{
	return "EndIDLE";
}

void UCBossAnimNotify_IDLE::Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::Notify(MeshComp, Animation);

	CheckNull ( MeshComp );
	CheckNull ( MeshComp->GetOwner ( ) );

	ACBossEnemy* My = Cast<ACBossEnemy> ( MeshComp->GetOwner ( ) );

	CheckNull ( My );

	UCBossAnim* MyAnim = Cast<UCBossAnim>(My->AnimInstance);
	CheckNull ( MyAnim );

	MyAnim->IsPlayingIdle = true;
}
