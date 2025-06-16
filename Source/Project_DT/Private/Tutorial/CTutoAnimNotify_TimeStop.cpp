// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoAnimNotify_TimeStop.h"
#include "Global.h"
#include "Tutorial/CTutorialEnemy.h"
#include "LHW_GameModeBase.h"
#include "Widget/CTutorialWidget.h"

FString UCTutoAnimNotify_TimeStop::GetNotifyName_Implementation() const
{
	return "TimeStop";
}

void UCTutoAnimNotify_TimeStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACTutorialEnemy* My = Cast<ACTutorialEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	// 글로벌 타임 스케일을 0으로 설정하여 시간 정지 효과 구현
	ALHW_GameModeBase* GameMode = Cast<ALHW_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
	{
		if (My->IsParry)
		{
			GameMode->TutorialWidget->SetSwitcherIndex(1);

			UGameplayStatics::SetGlobalTimeDilation(My->Target->GetController()->GetWorld(), 0.0f);
		}

		else
		{
			GameMode->TutorialWidget->SetSwitcherIndex(2);

			UGameplayStatics::SetGlobalTimeDilation(My->Target->GetController()->GetWorld(), 0.0f);
		}
	}
}
