// Fill out your copyright notice in the Description page of Project Settings.


#include "LHW_GameModeBase.h"
#include "Global.h"
#include "NetworkMessage.h"
#include "Component/CMointageComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Sound/SoundCue.h"
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
	CHelpers::GetAsset(&IntroSound, AssetPaths::IntroSound);
	CHelpers::GetAsset(&MainSound, AssetPaths::MainSound);

	// 생성자에서 오디오 컴포넌트를 미리 생성합니다.
	// 이는 GameMode 액터가 스폰될 때 함께 생성됩니다.
	BackgroundMusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundMusicComponent"));
	// GameMode는 Pawn이 아니므로 SetupAttachment가 필요 없을 수도 있습니다.
	// 하지만 만약을 위해 AddToRoot()를 사용하거나, UGameplayStatics::CreateSound2D를 사용하면 됩니다.
	// 여기서는 AMyGameModeBase가 액터이므로 컴포넌트가 자동으로 관리됩니다.
	BackgroundMusicComponent->bAutoActivate = false; // 자동 재생 안함
	BackgroundMusicComponent->bAllowSpatialization = false; // 2D 사운드
	BackgroundMusicComponent->bIsUISound = true; // UI 사운드 (옵션)

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
	PlayMusic_FadeIn(IntroSound,3.0f,0.5f);
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
	StopMusic_FadeOut(3.0f);
	IntroWidget->IsClick=true;
	LoadingWidget->AddToViewport();
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	FInputModeGameOnly InputMode;
	C->SetInputMode(InputMode);
	C->bShowMouseCursor=false;
	ACPlayer* player=Cast<ACPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	player->TestHandler();
}

void ALHW_GameModeBase::EndLoading()
{
	LoadingWidget->RemoveFromParent();
	OptionWidget->AddToViewport();
	OptionWidget->SetSwitcherIndex(0);
	TutorialWidget->AddToViewport();
	TutorialWidget->SetSwitcherIndex(0);
	PlayMusic_FadeIn(MainSound,6.0f,0.2f);
}

void ALHW_GameModeBase::CreateCharacterUI()
{
	PlayerWidget= CHelpers::GetWidget<UCPlayerWidget>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerWidget->AllowChange=true;
	PlayerWidget->AddToViewport();

	auto* Status= CHelpers::GetComponent<UCStatusComponent>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Status->Heal(50);
	// Status->Damage(100);
	Status->RecoverMana(100);

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

void ALHW_GameModeBase::HiddenQuest()
{
	QuestWidget->HiddenQuest();
}
void ALHW_GameModeBase::PlayMusic_FadeIn(USoundCue* SoundToPlay, float FadeInDuration, float VolumeMultiplier)
{
	if (!SoundToPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMyGameModeBase::PlayMusic_FadeIn - SoundToPlay is null."));
		return;
	}

	if (BackgroundMusicComponent)
	{
		// 현재 재생 중인 음악이 있다면 정지 (크로스페이드가 아니라면)
		if (BackgroundMusicComponent->IsPlaying() && BackgroundMusicComponent->Sound != SoundToPlay)
		{
			BackgroundMusicComponent->Stop();
		}

		BackgroundMusicComponent->SetSound(SoundToPlay); // 새 사운드 큐 할당
		BackgroundMusicComponent->SetVolumeMultiplier(VolumeMultiplier); // 볼륨 설정
		BackgroundMusicComponent->FadeIn(FadeInDuration, 1.0f, 0.0f); // 0.0 볼륨부터 1.0 볼륨까지 페이드 인
		BackgroundMusicComponent->Play(); // 페이드 인 전에 Play를 호출해야 합니다.
	}
}

void ALHW_GameModeBase::StopMusic_FadeOut(float FadeOutDuration)
{
	if (BackgroundMusicComponent && BackgroundMusicComponent->IsPlaying())
	{
		BackgroundMusicComponent->FadeOut(FadeOutDuration, 0.0f); // 현재 음악 페이드 아웃
	}
}