// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CScriptWidget.h"
#include "Components/Button.h"

void UCScriptWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button->OnClicked.AddDynamic(this,&UCScriptWidget::EndFadeOut);
	

}

void UCScriptWidget::EndFadeOut()
{
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	C->bShowMouseCursor=false;
	PlayAnimation(FadeOut, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);

}

void UCScriptWidget::StartFadeInAnimation()
{
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	C->bShowMouseCursor=true;
	PlayAnimation(FadeIn, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCScriptWidget::CreateScriptWidget()
{
	AddToViewport();
	StartFadeInAnimation();
}
