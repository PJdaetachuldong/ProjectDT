// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CIntroWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCIntroWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent FButtonClickEventEndDelegate;
	FWidgetAnimationDynamicEvent FPadeInEndDelegate;
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;


private:
	UFUNCTION()
	void PlayButtonAnimation();
	UFUNCTION()
	void PlayFadeInAnimation(); 
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* ButtonTextFadeIn;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* ButtonClickEvent;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* PadeIn;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UButton* Button;
public:
	bool IsClick=true;

};
