// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_ATKSwing3.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_ATKSwing3::GetNotifyName_Implementation() const
{
	return "SwingSound3";
}

void UCBossAnimNotify_ATKSwing3::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->SwordSwing_3)
	{
		UGameplayStatics::PlaySoundAtLocation(My, My->SwordSwing_3, My->GetActorLocation());
	}
}
