// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
public:
	void MapUIAnimation();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* FadeInOut;
};
