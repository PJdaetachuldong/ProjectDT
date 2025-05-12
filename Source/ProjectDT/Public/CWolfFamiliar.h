// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CFamiliarBase.h"
#include "CWolfFamiliar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDT_API ACWolfFamiliar : public ACFamiliarBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

public://오버라이드 할 것 같은 항목
	virtual void OnWeaponChanged();	// 무기에 따른 소환수 변화
	virtual void AssistAttack();	// 보조 공격
	virtual void SetTarget();		// 타겟 지정

public:// 늑대 클래스에서만 쓸 것 같은 항목


};
