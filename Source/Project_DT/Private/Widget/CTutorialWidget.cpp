// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CTutorialWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CPlayer.h"
#include "Component/CMointageComponent.h"
#include "Utilities/CHelper.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UCTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UCTutorialWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (WidgetSwitcher_9->GetActiveWidgetIndex() == 1 && InKeyEvent.GetKey() == EKeys::R)
	{
		SetSwitcherIndex(0);
		APlayerController* C = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		ACPlayer* Player = Cast<ACPlayer>(C->GetPawn());
		Player->Parry->OnParry();
		C->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		C->SetInputMode(InputMode);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		return FReply::Handled();
	}

	else if(WidgetSwitcher_9->GetActiveWidgetIndex() == 2 && InKeyEvent.GetKey() == EKeys::SpaceBar)
	{
		SetSwitcherIndex(0);
		APlayerController* C = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		UCMointageComponent* Montages=CHelpers::GetComponent<UCMointageComponent>(C->GetPawn());
		UCStateComponent* State=CHelpers::GetComponent<UCStateComponent>(C->GetPawn());
		State->SetBackStepMode();
		Montages->PlayBackStepMode(EActState::DodgeB);
		
		C->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		C->SetInputMode(InputMode);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCTutorialWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	// 현재 포커스된 Slate 위젯 가져오기(0은 사용자 인덱스, 멀티플레이어에서는 조정 가능)
// 		TSharedPtr<SWidget> FocusedSlateWidget = FSlateApplication::Get().GetUserFocusedWidget(0);
// 
// 	if (FocusedSlateWidget.IsValid())
// 	{
// 		// 위젯의 이름을 FString으로 변환하여 출력
// 		FString WidgetName = FocusedSlateWidget->ToString();
// 		UE_LOG(LogTemp, Log, TEXT("Focused Widget Name: %s"), *WidgetName);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("No widget is currently focused."));
// 	}
}

FReply UCTutorialWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 마우스 클릭 시 포커스를 다시 설정
	APlayerController* C = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	SetFocus();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(TakeWidget());
	C->SetInputMode(InputMode);
	return FReply::Handled(); // 마우스 입력을 캡처하여 뷰포트로 전달되지 않도록
}

void UCTutorialWidget::SetSwitcherIndex(int32 index)
{
	WidgetSwitcher_9->SetActiveWidgetIndex(index);

	switch (index)
	{
	case 0:
	{
	}break;
	case 1:
	{
		APlayerController* C = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		C->SetInputMode(InputMode);
	}break;
	case 2:
	{
		APlayerController* C = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		C->SetInputMode(InputMode);
	}break;
	case 3:
	{
		
	}break;
	case 4:
	{
		
	}break;
	default:
	{
	}break;
	}
}
