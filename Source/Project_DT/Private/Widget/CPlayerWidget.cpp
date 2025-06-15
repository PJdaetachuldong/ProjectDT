// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPlayerWidget.h"

#include "GameFramework/Character.h"
#include "Component/CStatusComponent.h"
#include "Component/CStateComponent.h"
#include "Global.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Weapons/CWeaponComponent.h"

void UCPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (GetWorld())
		OwnerCharacter=Cast<ACharacter>(OwnerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()));
	if (OwnerCharacter)
		Status=CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
		Weapon=CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
		State=CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	
	Status->OnDelegateHP.AddDynamic(this,&UCPlayerWidget::SetHpProgessBar);
	Status->OnDelegateMana.AddDynamic(this,&UCPlayerWidget::SetManaProgessBar);
	
	KatanaEndDelegate.BindDynamic(this,&UCPlayerWidget::KatanaEndSelect);
	GreatSwordEndDelegate.BindDynamic(this,&UCPlayerWidget::GreatSwordEndSelect);
	
	KatanaStartDelegate.BindDynamic(this,&UCPlayerWidget::CancelHandler);
	GreatSwordStartDelegate.BindDynamic(this,&UCPlayerWidget::CancelHandler);

	WeaponChangeEnd.BindDynamic(this,&UCPlayerWidget::TurnOnLight);

	BindToAnimationFinished(WeaponGageRotationKatana, WeaponChangeEnd);
	BindToAnimationFinished(WeaponGageRotationGreatSword, WeaponChangeEnd);
	
	
	BindToAnimationFinished(SelectSecondAnimation, KatanaEndDelegate);
	BindToAnimationFinished(SelectThirdAnimation, GreatSwordEndDelegate);

	BindToAnimationStarted(SelectSecondAnimation, KatanaStartDelegate);
	BindToAnimationStarted(SelectThirdAnimation, GreatSwordStartDelegate);
	if (ArrowAnimation)
		PlayAnimation(ArrowAnimation, 0.f, 0, EUMGSequencePlayMode::Forward, 1.0f);
	ShowStatusUI();
}

void UCPlayerWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if (WeaponSelect->GetRenderOpacity()>=0.05f)
		IsAllowChangeWeapon=true;
	else
		IsAllowChangeWeapon=false;
	// Red HP 부드럽게 감소
	if (!FMath::IsNearlyEqual(Red_Hp, TargetRed_Hp, 0.001f))
	{
		Red_Hp = FMath::Lerp(Red_Hp, TargetRed_Hp, DeltaTime * RedHpLerpSpeed);
		if (Player_HP_Red)
		{
			Player_HP_Red->SetPercent(Red_Hp);
		}
	}

	// Dark HP 더 느리게 감소
	if (!FMath::IsNearlyEqual(Dark_Hp, TargetDark_Hp, 0.001f))
	{
		Dark_Hp = FMath::Lerp(Dark_Hp, TargetDark_Hp, DeltaTime * DarkHpLerpSpeed);
		if (Player_HP_Dark)
		{
			Player_HP_Dark->SetPercent(Dark_Hp);
		}
	}

	// Sword Mana 부드럽게 감소
	if (!FMath::IsNearlyEqual(Sword_Mana, TargetMana, 0.001f))
	{
		Sword_Mana = FMath::Lerp(Sword_Mana, TargetMana, DeltaTime * ManaLerpSpeed);
		if (Sword_Energy)
		{
			Sword_Energy->SetPercent(Sword_Mana);
		}
	}
}

void UCPlayerWidget::SetHpProgessBar(float HP)
{
	TargetRed_Hp = FMath::Clamp(HP / 100.0f, 0.0f, 1.0f);
	TargetDark_Hp = TargetRed_Hp;
}

void UCPlayerWidget::SetManaProgessBar(float Mana)
{
	TargetMana = FMath::Clamp(Mana / 100.0f, 0.0f, 1.0f);
}

void UCPlayerWidget::FadeInSelectWindow()
{
	PlayAnimation(WeaponSelectPadeAnimation);
}

void UCPlayerWidget::FadeOutSelectWindow()
{
		PlayAnimation(WeaponSelectPadeOutAnimation);
}

void UCPlayerWidget::SelectKatana()
{
	CheckTrue(Weapon->IsKatanaMode());
	// if (GetIsAllowChangeWeapon())
		PlayAnimation(SelectSecondAnimation);
}

void UCPlayerWidget::SelectGreatSword()
{
	CheckTrue(Weapon->IsGreatSwordMode());
	// if (GetIsAllowChangeWeapon())
		PlayAnimation(SelectThirdAnimation);
}

void UCPlayerWidget::KatanaEndSelect()
{
	CheckNull(State);
	State->SetIdleMode();
	Weapon->SetKatanaMode();

	IsCancelWidget=false;
	UGameplayStatics::SetGlobalTimeDilation(OwnerCharacter, 1.0f);
	PlayAnimation(WeaponGageRotationKatana);
}

void UCPlayerWidget::GreatSwordEndSelect()
{
	CheckNull(State);
	State->SetIdleMode();
	Weapon->SetGreatSwordMode();
	IsCancelWidget=false;
	UGameplayStatics::SetGlobalTimeDilation(OwnerCharacter, 1.0f);
	PlayAnimation(WeaponGageRotationGreatSword);
}

void UCPlayerWidget::CancelHandler()
{
	IsCancelWidget=true;
	CheckNull(State);
	State->SetCancelMode();
}

void UCPlayerWidget::TurnOnLight()
{
	PlayAnimation(LightOnOff, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	FadeOutSelectWindow();
}

void UCPlayerWidget::ShowStatusUI()
{
	PlayAnimation(StatusFadeIn, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}
