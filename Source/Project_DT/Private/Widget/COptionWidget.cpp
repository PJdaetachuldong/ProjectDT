// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/COptionWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UCOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	resume->OnClicked.AddDynamic(this,&UCOptionWidget::ResumeHandler);
	Key->OnClicked.AddDynamic(this,&UCOptionWidget::KeyHandler);
	Teleport->OnClicked.AddDynamic(this,&UCOptionWidget::TeleportHandler);
	Exit->OnClicked.AddDynamic(this,&UCOptionWidget::ExitHandler);
}

FReply UCOptionWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		SetSwitcher();
		return FReply::Handled(); 
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCOptionWidget::ResumeHandler()
{
	SetSwitcher();
}

void UCOptionWidget::KeyHandler()
{
	SetSwitcherIndex(2);
}

void UCOptionWidget::TeleportHandler()
{
}

void UCOptionWidget::ExitHandler()
{
	APlayerController* PlayerController=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	UKismetSystemLibrary::QuitGame(
	GetWorld(),
	PlayerController,
	EQuitPreference::Quit,
	false
);
}

void UCOptionWidget::SetSwitcher()

{

	switch (Switcher->GetActiveWidgetIndex())

	{

	case 0:

		{

			Switcher->SetActiveWidgetIndex(1);

			APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

			C->bShowMouseCursor=true;

			FInputModeUIOnly InputMode;

			InputMode.SetWidgetToFocus(TakeWidget());

			C->SetInputMode(InputMode);

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);

		}break;

	case 1:

		{

			Switcher->SetActiveWidgetIndex(0);

			APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

			C->bShowMouseCursor=false;

			FInputModeGameOnly InputMode;

			C->SetInputMode(InputMode);

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		}break;

	case 2:

		{

			Switcher->SetActiveWidgetIndex(1);

		}break;

	default:

		{

			// Switcher->SetActiveWidgetIndex(0);

			// APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

			// C->bShowMouseCursor=true;

			// FInputModeGameOnly InputMode;

			// C->SetInputMode(InputMode);

			// UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		}break;

	}

}
void UCOptionWidget::SetSwitcherIndex(int32 index)
{
	Switcher->SetActiveWidgetIndex(index);
}
