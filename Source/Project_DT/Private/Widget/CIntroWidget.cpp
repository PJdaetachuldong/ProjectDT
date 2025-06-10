// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CIntroWidget.h"

#include "Components/Button.h"

void UCIntroWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button->OnClicked.AddDynamic(this,&UCIntroWidget::PlayButtonAnimation);
	PlayAnimation(ButtonTextFadeIn, 0.f, 0, EUMGSequencePlayMode::Forward, 1.0f);
	
	FButtonClickEventEndDelegate.BindDynamic(this,&UCIntroWidget::PlayFadeInAnimation);
	BindToAnimationFinished(ButtonClickEvent, FButtonClickEventEndDelegate);

	FPadeInEndDelegate.BindDynamic(this,&UCIntroWidget::PlayFadeInAnimation);
	BindToAnimationFinished(PadeIn, FButtonClickEventEndDelegate);
	
}

FReply UCIntroWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	PlayButtonAnimation();
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCIntroWidget::PlayButtonAnimation()
{
	StopAnimation(ButtonTextFadeIn);
	PlayAnimation(ButtonClickEvent, 0.f, 1, EUMGSequencePlayMode::Forward, 1.22f);
	
}

void UCIntroWidget::PlayFadeInAnimation()
{
	PlayAnimation(PadeIn, 0.f, 1, EUMGSequencePlayMode::Forward, 1.22f);
}
