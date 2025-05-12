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

public://�������̵� �� �� ���� �׸�
	virtual void OnWeaponChanged();	// ���⿡ ���� ��ȯ�� ��ȭ
	virtual void AssistAttack();	// ���� ����
	virtual void SetTarget();		// Ÿ�� ����

public:// ���� Ŭ���������� �� �� ���� �׸�


};
