// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

public:
	UFUNCTION()
	void SetBossHpProgessBar(float HP);
	UFUNCTION()
	void SetBossShieldProgessBar(float S);
	void HiddenProgressbar();
	void SetOwner(AActor* Owner);
private:
	class ACBossEnemy* BossOwner;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	float Dark_Hp=1.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	float Red_Hp=1.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	float Shield=1.0;
	
};


