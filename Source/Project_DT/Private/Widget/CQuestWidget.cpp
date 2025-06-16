// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CQuestWidget.h"

void UCQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCQuestWidget::ShowQuest()
{
	AddToViewport();
	PlayAnimation(QuestFadeIn);
}

void UCQuestWidget::HiddenQuest()
{
	PlayAnimationReverse(QuestFadeIn);
}
