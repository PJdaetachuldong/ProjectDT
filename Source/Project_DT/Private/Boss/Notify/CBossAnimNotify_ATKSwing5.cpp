// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_ATKSwing5.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_ATKSwing5::GetNotifyName_Implementation() const
{
	return "SwingSound5";
}

void UCBossAnimNotify_ATKSwing5::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->SwordSwing_5)
	{
		UGameplayStatics::PlaySoundAtLocation(My, My->SwordSwing_5, My->GetActorLocation());
	}
}
