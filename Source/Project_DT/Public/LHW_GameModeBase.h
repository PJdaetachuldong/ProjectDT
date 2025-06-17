// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LHW_GameModeBase.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_DT_API ALHW_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	ALHW_GameModeBase();

	// 게임 시작 시 호출
	virtual void BeginPlay() override;

	// 매 프레임 호출
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void CreateLoadingUI();

	UFUNCTION()
	void EndLoading();

	UFUNCTION(BlueprintCallable)
	void CreateCharacterUI();

	UFUNCTION(BlueprintCallable)
	void CreateScriptUI(const FString& Text);

	UFUNCTION()
	void CreateMapUI();

	void SwitchOption();
	UFUNCTION(BlueprintCallable)
	void HiddenQuest();
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopMusic_FadeOut(float FadeOutDuration = 2.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayMusic_FadeIn(USoundCue* SoundToPlay, float FadeInDuration = 2.0f, float VolumeMultiplier = 1.0f);

private:
	bool CheckMap=true;
	
private:

	UPROPERTY()
	class UAudioComponent* BackgroundMusicComponent;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>IntroWidgetClass;

	UPROPERTY(EditAnywhere)
	class UCIntroWidget* IntroWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>LoadingWidgetClass;

	UPROPERTY(EditAnywhere)
	class UCLoadingWidget* LoadingWidget;

	UPROPERTY(EditAnywhere)
	class UCPlayerWidget* PlayerWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>ScriptWidgetClass;

	UPROPERTY(EditAnywhere)
	class UCScriptWidget* ScriptWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>MapWidgetClass;

	UPROPERTY(EditAnywhere)
	class UCMapWidget* MapWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>QuestWidgetClass;

	UPROPERTY(EditAnywhere)
	class UCQuestWidget* QuestWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>OptionWidgetClass;

	UPROPERTY(EditAnywhere)
	class UCOptionWidget* OptionWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>TutorialWidgetClass;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USoundCue* IntroSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USoundCue* MainSound;

public:
	UPROPERTY(EditAnywhere)
	class UCTutorialWidget* TutorialWidget;

};
