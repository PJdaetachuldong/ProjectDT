// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CLoadingWidget.h"

void UCLoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FCompleteProgressBarAnimation.BindDynamic(this,&UCLoadingWidget::FadeOutEndAnimation);
	BindToAnimationFinished(ProgressBarGageAnimation, FCompleteProgressBarAnimation);
	
	PlayAnimation(MarkAnimation, 0.f, 0, EUMGSequencePlayMode::Forward, 0.8f);
	PlayAnimation(ProgressBarAnimation, 0.f, 0, EUMGSequencePlayMode::Forward, 0.4f);
	PlayAnimation(ProgressBarGageAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCLoadingWidget::FadeOutEndAnimation()
{
	PlayAnimation(FadeOut, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

