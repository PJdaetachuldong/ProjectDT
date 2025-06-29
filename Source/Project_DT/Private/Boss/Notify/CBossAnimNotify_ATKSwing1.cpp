// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_ATKSwing1.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_ATKSwing1::GetNotifyName_Implementation() const
{
	return "SwingSound1";
}

void UCBossAnimNotify_ATKSwing1::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	if (My->SwordSwing_1)
	{
		UGameplayStatics::PlaySoundAtLocation(My,My->SwordSwing_1,My->GetActorLocation());
	}
}
