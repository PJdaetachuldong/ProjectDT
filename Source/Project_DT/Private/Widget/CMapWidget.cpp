// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CMapWidget.h"

void UCMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCMapWidget::MapUIAnimation()
{
	AddToViewport();
	PlayAnimation(FadeInOut, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}
