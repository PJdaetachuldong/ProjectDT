// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "COptionWidget.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_DT_API UCOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;


public:
	UFUNCTION(BlueprintCallable)
	void ResumeHandler();
	UFUNCTION(BlueprintCallable)
	void KeyHandler();
	UFUNCTION(BlueprintCallable)
	void TeleportHandler();
	UFUNCTION(BlueprintCallable)
	void ExitHandler();
	UFUNCTION(BlueprintCallable)
	void SetSwitcher();
	UFUNCTION(BlueprintCallable)
	void SetSwitcherIndex(int32 index);

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UButton* resume;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UButton* Key;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UButton* Teleport;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UButton* Exit;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UCanvasPanel* Empty;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UCanvasPanel* Option;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UCanvasPanel* KeyTutorial;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UWidgetSwitcher* Switcher;

	UPROPERTY(BlueprintReadOnly, Category = "Position", meta = (BindWidget))
	FVector Position= FVector(-3872.0, -13410.0, 960.0);

};
