// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_ATKSwing2.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_ATKSwing2::GetNotifyName_Implementation() const
{
	return "SwingSound2";
}

void UCBossAnimNotify_ATKSwing2::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->SwordSwing_2)
	{
		UGameplayStatics::PlaySoundAtLocation(My, My->SwordSwing_2, My->GetActorLocation());
	}
}
