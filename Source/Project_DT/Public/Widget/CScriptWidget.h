// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CScriptWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCScriptWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FWidgetAnimationDynamicEvent FCompleteFadeOutAnimation;
	FWidgetAnimationDynamicEvent FCompleteFadeInAnimation;
public:
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void EndFadeOut();
public:
	void StartFadeInAnimation();
	UFUNCTION()
	void CreateScriptWidget();
	UFUNCTION(BlueprintCallable)
	void SetText(const FString& FullText);
	UFUNCTION()
	void StartTypingEffect();
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* FadeIn;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* FadeOut;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UButton* Button;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UTextBlock* TypeText;

private:
	FString FullText;
	FTimerHandle TypingTimerHandle;
	
};
