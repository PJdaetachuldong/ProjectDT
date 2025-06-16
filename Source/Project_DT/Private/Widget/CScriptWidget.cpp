// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CScriptWidget.h"

#include "Component/CMovementComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Utilities/CHelper.h"

void UCScriptWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button->OnClicked.AddDynamic(this,&UCScriptWidget::EndFadeOut);

	FCompleteFadeInAnimation.BindDynamic(this,&UCScriptWidget::StartTypingEffect);
	BindToAnimationFinished(FadeIn, FCompleteFadeInAnimation);
	Arrow->SetRenderOpacity(0);

}

FReply UCScriptWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// === 1. 눌린 키가 'F' 키인지 확인 ===
	if (InKeyEvent.GetKey() == EKeys::F)
	{
		EndFadeOut();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCScriptWidget::EndFadeOut()
{
	if (bIsTypingEffectActive)return;
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	UCMovementComponent* Movement= CHelpers::GetComponent<UCMovementComponent>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Movement)
		Movement->Move();
	C->bShowMouseCursor=false;
	FInputModeGameOnly InputMode;
	C->SetInputMode(InputMode);
	PlayAnimation(FadeOut, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	FullText="";
	TypeText->SetText(FText::FromString(FullText));
	Arrow->SetRenderOpacity(0);


}

void UCScriptWidget::StartFadeInAnimation()
{
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	UCMovementComponent* Movement= CHelpers::GetComponent<UCMovementComponent>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Movement)
		Movement->Stop();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(TakeWidget());
	C->SetInputMode(InputMode);
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
	bIsTypingEffectActive = true;
	GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, FTimerDelegate::CreateLambda([this, CurrentIndex]() mutable {

		if (CurrentIndex >= FullText.Len())
		{
			bIsTypingEffectActive = false;
			Arrow->SetRenderOpacity(1);
			return;
		}

		FString Substring = FullText.Left(CurrentIndex + 1);
		TypeText->SetText(FText::FromString(Substring));
		CurrentIndex++;

	}), 0.05f, true); // 속도는 조절 가능
}
void UCScriptWidget::NativeDestruct()
{
	Super::NativeDestruct(); // 부모 함수 호출

	// 위젯이 파괴될 때 타이머를 명시적으로 클리어합니다.
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TypingTimerHandle);
	}
}