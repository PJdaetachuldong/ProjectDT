// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCLoadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent FCompleteProgressBarAnimation;
	FWidgetAnimationDynamicEvent FCompleteFadeOutAnimation;
	
public:
	virtual void NativeConstruct() override;
public:
	UFUNCTION()
	void FadeOutEndAnimation();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* MarkAnimation;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* ProgressBarAnimation;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* FadeOut;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* ProgressBarGageAnimation;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UProgressBar* LoadingProgressBar;
	
	
};
