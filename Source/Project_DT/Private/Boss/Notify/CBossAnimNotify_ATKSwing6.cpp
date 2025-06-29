// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_ATKSwing6.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_ATKSwing6::GetNotifyName_Implementation() const
{
	return "SwingSound6";
}

void UCBossAnimNotify_ATKSwing6::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->SwordSwing_6)
	{
		UGameplayStatics::PlaySoundAtLocation(My, My->SwordSwing_6, My->GetActorLocation());
	}
}
