// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_CounterTutorial.h"
#include "Global.h"
#include "LHW_GameModeBase.h"
#include "Components/Image.h"
#include "Widget/CPlayerWidget.h"
#include "Widget/CTutorialWidget.h"

FString UCAnimNotifyState_CounterTutorial::GetNotifyName_Implementation() const
{
	return "CounterTutorial";
}

void UCAnimNotifyState_CounterTutorial::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp->GetOwner());
	UCPlayerWidget* widget=CHelpers::GetWidget<UCPlayerWidget>(MeshComp->GetOwner());
	CheckNull(widget);
	widget->Board->SetRenderOpacity(0.2);
	
}

void UCAnimNotifyState_CounterTutorial::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());
	UCPlayerWidget* widget=CHelpers::GetWidget<UCPlayerWidget>(MeshComp->GetOwner());
	CheckNull(widget);
	widget->Board->SetRenderOpacity(0);
}
