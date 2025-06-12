// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	FWidgetAnimationDynamicEvent GameOverUIEnd;
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* GameOverPadeIn;
	
	
};
