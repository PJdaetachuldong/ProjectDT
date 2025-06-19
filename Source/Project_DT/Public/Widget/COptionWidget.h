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
	UFUNCTION(BlueprintCallable)
	void ShowBalance();

	UFUNCTION(BlueprintCallable)
	void SetBossBalance();

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
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UButton* Balance;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class URadialSlider* HPSlider;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class URadialSlider* ShieldSlider;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UTextBlock* BossHPText;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UTextBlock* BossShieldText;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UButton* Set;

	UPROPERTY(BlueprintReadOnly, Category = "Position", meta = (BindWidget))
	FVector Position= FVector(-3872.0, -13410.0, 960.0);

private:
	class ACBossEnemy* BossEnemy;

private:
	UFUNCTION()
	void HpValueChanged(float v);
	UFUNCTION()
	void ShieldValueChanged(float v);

	UPROPERTY()
	float HP; // 현재 슬라이더 값을 저장할 변수
	UPROPERTY()
	float Shield; // 현재 슬라이더 값을 저장할 변수

	UPROPERTY()
	float MaxHPValue = 600.0f; // 최대 체력 설정 값
	UPROPERTY()
	float MaxShieldValue = 600.0f; // 최대 실드 설정 값
	

};
