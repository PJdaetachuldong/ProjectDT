// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CTutorialWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CPlayer.h"

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
		ACPlayer* Player = Cast<ACPlayer>(C->GetPawn());
		Player->OnAvoid();
		C->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		C->SetInputMode(InputMode);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
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
