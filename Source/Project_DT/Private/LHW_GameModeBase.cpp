// Fill out your copyright notice in the Description page of Project Settings.


#include "LHW_GameModeBase.h"
#include "Global.h"
#include "Component/CMointageComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Widget/CGameOverWidget.h"
#include "Widget/CIntroWidget.h"
#include "Widget/CLoadingWidget.h"
#include "Widget/CPlayerWidget.h"
#include "Widget/CScriptWidget.h"
#include "Widget/CMapWidget.h"
#include "Widget/COptionWidget.h"
#include "Widget/CQuestWidget.h"
#include "Widget/CTutorialWidget.h"


ALHW_GameModeBase::ALHW_GameModeBase()
{
	CHelpers::GetClass(&IntroWidgetClass,AssetPaths::IntroUI);
	CHelpers::GetClass(&LoadingWidgetClass,AssetPaths::LoadingUI);
	CHelpers::GetClass(&ScriptWidgetClass,AssetPaths::ScriptUI);
	CHelpers::GetClass(&MapWidgetClass,AssetPaths::MapWidget);
	CHelpers::GetClass(&QuestWidgetClass,AssetPaths::QuestWidget);
	CHelpers::GetClass(&QuestWidgetClass,AssetPaths::QuestWidget);
	CHelpers::GetClass(&OptionWidgetClass,AssetPaths::OptionWidget);
	CHelpers::GetClass(&TutorialWidgetClass, AssetPaths::Tutorial);

}

void ALHW_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//위젯 생성
	IntroWidget=CreateWidget<UCIntroWidget>(GetWorld(),IntroWidgetClass);
	LoadingWidget=CreateWidget<UCLoadingWidget>(GetWorld(),LoadingWidgetClass);
	ScriptWidget=CreateWidget<UCScriptWidget>(GetWorld(),ScriptWidgetClass);
	MapWidget=CreateWidget<UCMapWidget>(GetWorld(),MapWidgetClass);
	QuestWidget=CreateWidget<UCQuestWidget>(GetWorld(),QuestWidgetClass);
	OptionWidget=CreateWidget<UCOptionWidget>(GetWorld(),OptionWidgetClass);
	TutorialWidget = CreateWidget<UCTutorialWidget>(GetWorld(), TutorialWidgetClass);

	//인트로 시작
	IntroWidget->AddToViewport();

	IntroWidget->FPadeInEndDelegate.BindDynamic(this,&ALHW_GameModeBase::CreateLoadingUI);
	IntroWidget->BindToAnimationFinished(IntroWidget->PadeIn, IntroWidget->FPadeInEndDelegate);
	//로딩 바인드
	LoadingWidget->FCompleteFadeOutAnimation.BindDynamic(this,&ALHW_GameModeBase::EndLoading);
	LoadingWidget->BindToAnimationFinished(LoadingWidget->FadeOut, LoadingWidget->FCompleteFadeOutAnimation);

	ScriptWidget->FCompleteFadeOutAnimation.BindDynamic(this,&ALHW_GameModeBase::CreateMapUI);
	ScriptWidget->BindToAnimationFinished(ScriptWidget->FadeOut, ScriptWidget->FCompleteFadeOutAnimation);

	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	C->bShowMouseCursor=false;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(IntroWidget->TakeWidget());
	C->SetInputMode(InputMode);
}

void ALHW_GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALHW_GameModeBase::CreateLoadingUI()
{
	IntroWidget->RemoveFromParent();
	IntroWidget->IsClick=true;
	LoadingWidget->AddToViewport();
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	FInputModeGameOnly InputMode;
	C->SetInputMode(InputMode);
	C->bShowMouseCursor=false;
}

void ALHW_GameModeBase::EndLoading()
{
	LoadingWidget->RemoveFromParent();
	OptionWidget->AddToViewport();
	OptionWidget->SetSwitcherIndex(0);
	TutorialWidget->AddToViewport();
	TutorialWidget->SetSwitcherIndex(0);
}

void ALHW_GameModeBase::CreateCharacterUI()
{
	PlayerWidget= CHelpers::GetWidget<UCPlayerWidget>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerWidget->AllowChange=true;
	PlayerWidget->AddToViewport();

	auto* Status= CHelpers::GetComponent<UCStatusComponent>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Status->Heal(100);
	// Status->Damage(100);
	Status->UseMana(100);

	FTimerHandle Handler;
}

void ALHW_GameModeBase::CreateScriptUI(const FString& Text)
{
	ScriptWidget->SetText(Text);
	ScriptWidget->CreateScriptWidget();
}

void ALHW_GameModeBase::CreateMapUI()
{
	ScriptWidget->RemoveFromParent();
	if (!CheckMap)return;
	CheckMap=false;
	QuestWidget->ShowQuest();
	MapWidget->MapUIAnimation();
}

void ALHW_GameModeBase::SwitchOption()
{
	if(OptionWidget->Switcher->GetActiveWidgetIndex()==0)
		OptionWidget->SetSwitcher();
}
