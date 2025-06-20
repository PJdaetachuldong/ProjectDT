// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	FWidgetAnimationDynamicEvent KatanaEndDelegate;
	FWidgetAnimationDynamicEvent GreatSwordEndDelegate;
	FWidgetAnimationDynamicEvent KatanaStartDelegate;
	FWidgetAnimationDynamicEvent GreatSwordStartDelegate;
	FWidgetAnimationDynamicEvent WeaponChangeEnd;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
public:
	UFUNCTION()
	void FadeInSelectWindow();
	UFUNCTION()
	void FadeOutSelectWindow();

	FORCEINLINE bool GetIsAllowChangeWeapon(){return IsAllowChangeWeapon;};
	FORCEINLINE bool GetIsCancelWidget(){return IsCancelWidget;};

	void SelectKatana();
	void SelectGreatSword();
	UFUNCTION()
	void KatanaEndSelect();
	UFUNCTION()
	void GreatSwordEndSelect();
	UFUNCTION()
	void CancelHandler();
	UFUNCTION()
	void TurnOnLight();

	void ShowStatusUI();

private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class UCStatusComponent* Status;
	UPROPERTY()
	class UCWeaponComponent* Weapon;
	UPROPERTY()
	class UCStateComponent* State;

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UImage* Board;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UProgressBar* Player_HP_Dark;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UProgressBar* Player_HP_Red;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UProgressBar* Sword_Energy;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	class UCanvasPanel* WeaponSelect;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* ArrowAnimation;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* SelectSecondAnimation;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* SelectThirdAnimation;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* WeaponSelectPadeAnimation;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* WeaponSelectPadeOutAnimation;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* WeaponGageRotationKatana;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* WeaponGageRotationGreatSword;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* StatusFadeIn;
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim),Transient,BlueprintReadWrite)
	class UWidgetAnimation* LightOnOff;

private:
	UFUNCTION()
	void SetHpProgessBar(float HP);
	UFUNCTION()
	void SetManaProgessBar(float Mana);



public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	float Dark_Hp=1.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	float Red_Hp=1.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	float Sword_Mana=0.0;
	float TargetRed_Hp;
	float TargetDark_Hp;
	float TargetMana;

private:
	bool IsAllowChangeWeapon=false;
	bool IsCancelWidget=false;

	UPROPERTY(EditAnywhere, Category = "UI")
	float RedHpLerpSpeed = 5.0f; // 빨간 체력바 감소 속도 (빠름)

	UPROPERTY(EditAnywhere, Category = "UI")
	float DarkHpLerpSpeed = 0.5f; // 다크 체력바 감소 속도 (느림)

	UPROPERTY(EditAnywhere, Category = "UI")
	float ManaLerpSpeed = 4.0f;
public:
	UPROPERTY()
	bool AllowChange=false;

};
