// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BossWidget.h"

#include "Boss/CBossEnemy.h"

void UBossWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBossWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
}

void UBossWidget::SetBossHpProgessBar(float HP)
{
	float hp = FMath::Clamp(HP / BossOwner->MaxHP, 0.0f, 1.0f);
	Dark_Hp=hp;
	Red_Hp=hp;
}

void UBossWidget::SetBossShieldProgessBar(float S)
{
	float shield = FMath::Clamp(S / BossOwner->MaxShieldAmount, 0.0f, 1.0f);
	Shield=shield;
}
void UBossWidget::SetOwner(AActor* Owner)
{
	BossOwner=Cast<ACBossEnemy>(Owner);
	BossOwner->OnDelegateHP.AddDynamic(this,&UBossWidget::SetBossHpProgessBar);
	BossOwner->OnDelegateShield.AddDynamic(this,&UBossWidget::SetBossShieldProgessBar);
}