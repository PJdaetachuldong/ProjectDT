// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoAnimNotify_TimeStop.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCTutoAnimNotify_TimeStop::GetNotifyName_Implementation() const
{
	return "TimeStop";
}

void UCTutoAnimNotify_TimeStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	// 글로벌 타임 스케일을 0으로 설정하여 시간 정지 효과 구현
	UGameplayStatics::SetGlobalTimeDilation(My->Target->GetController()->GetWorld(), 0.0f);
}
