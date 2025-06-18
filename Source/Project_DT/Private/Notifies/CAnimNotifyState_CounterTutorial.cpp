// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_CounterTutorial.h"
#include "Global.h"
#include "LHW_GameModeBase.h"
#include "Widget/CTutorialWidget.h"

FString UCAnimNotifyState_CounterTutorial::GetNotifyName_Implementation() const
{
	return "CounterTutorial";
}

void UCAnimNotifyState_CounterTutorial::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	ALHW_GameModeBase* GameMode=Cast<ALHW_GameModeBase>(GetWorld());
	CheckNull(GameMode);
		GameMode->TutorialWidget->SetSwitcherIndex(5);
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 0.8f);
	
}

void UCAnimNotifyState_CounterTutorial::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	ALHW_GameModeBase* GameMode=Cast<ALHW_GameModeBase>(MeshComp->GetOwner()->GetWorld());
	CheckNull(GameMode);
	GameMode->TutorialWidget->SetSwitcherIndex(0);
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 1.0f);
	
}
