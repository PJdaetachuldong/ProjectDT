// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CQuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCQuestWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void ShowQuest();
	void HiddenQuest();
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* QuestFadeIn;
};
