// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTutorialWidget.generated.h"

UCLASS()
class PROJECT_DT_API UCTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable)
	void SetSwitcherIndex(int32 index);

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_9;
};
