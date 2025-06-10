// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPlayerWidget.h"

#include "GameFramework/Character.h"
#include "Component/CStatusComponent.h"
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
	
	Status->OnDelegateHP.AddDynamic(this,&UCPlayerWidget::SetHpProgessBar);
	Status->OnDelegateMana.AddDynamic(this,&UCPlayerWidget::SetManaProgessBar);
	
	KatanaEndDelegate.BindDynamic(this,&UCPlayerWidget::KatanaEndSelect);
	GreatSwordEndDelegate.BindDynamic(this,&UCPlayerWidget::GreatSwordEndSelect);
	
	KatanaStartDelegate.BindDynamic(this,&UCPlayerWidget::CancelHandler);
	GreatSwordStartDelegate.BindDynamic(this,&UCPlayerWidget::CancelHandler);
	
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
	if (WeaponSelect->GetRenderOpacity()>=0.4f)
		IsAllowChangeWeapon=true;
	else
		IsAllowChangeWeapon=false;
}

void UCPlayerWidget::SetHpProgessBar(float HP)
{
	float hp = FMath::Clamp(HP / 100, 0.0f, 1.0f);
	Dark_Hp=hp;
	Red_Hp=hp;
}

void UCPlayerWidget::SetManaProgessBar(float Mana)
{
	float mana = FMath::Clamp(Mana / 100, 0.0f, 1.0f);
	Sword_Mana=mana;
}

void UCPlayerWidget::FadeInSelectWindow()
{
	PlayAnimation(WeaponSelectPadeAnimation);
}

void UCPlayerWidget::FadeOutSelectWindow()
{
	if (WeaponSelect->GetRenderOpacity()>0.1f)
		PlayAnimationReverse(WeaponSelectPadeAnimation);
}

void UCPlayerWidget::SelectKatana()
{
	CheckTrue(Weapon->IsKatanaMode());
	if (GetIsAllowChangeWeapon())
		PlayAnimation(SelectSecondAnimation);
}

void UCPlayerWidget::SelectGreatSword()
{
	CheckTrue(Weapon->IsGreatSwordMode());
	if (GetIsAllowChangeWeapon())
		PlayAnimation(SelectThirdAnimation);
}

void UCPlayerWidget::KatanaEndSelect()
{
	Weapon->SetKatanaMode();
	FadeOutSelectWindow();
	IsCancelWidget=false;
	UGameplayStatics::SetGlobalTimeDilation(OwnerCharacter, 1.0f);
	PlayAnimation(WeaponGageRotationKatana);
}

void UCPlayerWidget::GreatSwordEndSelect()
{
	Weapon->SetGreatSwordMode();
	FadeOutSelectWindow();
	IsCancelWidget=false;
	UGameplayStatics::SetGlobalTimeDilation(OwnerCharacter, 1.0f);
	PlayAnimation(WeaponGageRotationGreatSword);
}

void UCPlayerWidget::CancelHandler()
{
	IsCancelWidget=true;
}

void UCPlayerWidget::ShowStatusUI()
{
	PlayAnimation(StatusFadeIn, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCPlayerWidget::ShowQuestUI()
{
		PlayAnimation(QuestFadeIn, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}
