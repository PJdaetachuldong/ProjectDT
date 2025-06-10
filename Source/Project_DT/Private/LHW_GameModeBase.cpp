// Fill out your copyright notice in the Description page of Project Settings.


#include "LHW_GameModeBase.h"
#include "Global.h"
#include "Widget/CIntroWidget.h"
#include "Widget/CLoadingWidget.h"
#include "Widget/CPlayerWidget.h"
#include "Widget/CScriptWidget.h"
#include "Widget/CMapWidget.h"

ALHW_GameModeBase::ALHW_GameModeBase()
{
	CHelpers::GetClass(&IntroWidgetClass,AssetPaths::IntroUI);
	CHelpers::GetClass(&LoadingWidgetClass,AssetPaths::LoadingUI);
	CHelpers::GetClass(&ScriptWidgetClass,AssetPaths::ScriptUI);
	CHelpers::GetClass(&MapWidgetClass,AssetPaths::MapWidget);
}

void ALHW_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//위젯 생성
	IntroWidget=CreateWidget<UCIntroWidget>(GetWorld(),IntroWidgetClass);
	LoadingWidget=CreateWidget<UCLoadingWidget>(GetWorld(),LoadingWidgetClass);
	ScriptWidget=CreateWidget<UCScriptWidget>(GetWorld(),ScriptWidgetClass);
	MapWidget=CreateWidget<UCMapWidget>(GetWorld(),MapWidgetClass);
	//인트로 시작
	IntroWidget->AddToViewport();
	IntroWidget->FPadeInEndDelegate.BindDynamic(this,&ALHW_GameModeBase::CreateLoadingUI);
	IntroWidget->BindToAnimationFinished(IntroWidget->PadeIn, IntroWidget->FPadeInEndDelegate);
	//로딩 바인드
	LoadingWidget->FCompleteFadeOutAnimation.BindDynamic(this,&ALHW_GameModeBase::EndLoading);
	LoadingWidget->BindToAnimationFinished(LoadingWidget->FadeOut, LoadingWidget->FCompleteFadeOutAnimation);

	ScriptWidget->FCompleteFadeOutAnimation.BindDynamic(this,&ALHW_GameModeBase::CreateMapUI);
	ScriptWidget->BindToAnimationFinished(ScriptWidget->FadeOut, ScriptWidget->FCompleteFadeOutAnimation);
	
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
}

void ALHW_GameModeBase::EndLoading()
{
	LoadingWidget->RemoveFromParent();
	CreateCharacterUI();
}

void ALHW_GameModeBase::CreateCharacterUI()
{
	PlayerWidget= CHelpers::GetWidget<UCPlayerWidget>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerWidget->AddToViewport();
	
	//이부분 원할때 변경
	FTimerHandle Handler;
	GetWorld()->GetTimerManager().SetTimer(Handler,[this](){CreateScriptUI();},20,false,false);
	
}

void ALHW_GameModeBase::CreateScriptUI()
{
	ScriptWidget->CreateScriptWidget();
}

void ALHW_GameModeBase::CreateMapUI()
{
	ScriptWidget->RemoveFromParent();
	PlayerWidget->ShowQuestUI();
	MapWidget->MapUIAnimation();
}
