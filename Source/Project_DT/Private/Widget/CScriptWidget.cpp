// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CScriptWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCScriptWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button->OnClicked.AddDynamic(this,&UCScriptWidget::EndFadeOut);

	FCompleteFadeInAnimation.BindDynamic(this,&UCScriptWidget::StartTypingEffect);
	BindToAnimationFinished(FadeIn, FCompleteFadeInAnimation);
	

}

void UCScriptWidget::EndFadeOut()
{
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	C->bShowMouseCursor=false;
	PlayAnimation(FadeOut, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	FullText="";

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

void UCScriptWidget::SetText(const FString& Text)
{
	FullText=Text;
}

void UCScriptWidget::StartTypingEffect()
{
	if (!TypeText) return;

	TypeText->SetText(FText::FromString(TEXT("")));

	int32 CurrentIndex = 0;

	GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, FTimerDelegate::CreateLambda([this, CurrentIndex]() mutable {

		if (CurrentIndex >= FullText.Len())
		{
			GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle); // 클래스 멤버로 선언된 거!
			return;
		}

		FString Substring = FullText.Left(CurrentIndex + 1);
		TypeText->SetText(FText::FromString(Substring));
		CurrentIndex++;

	}), 0.05f, true); // 속도는 조절 가능
}
