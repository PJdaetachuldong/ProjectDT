// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_DT_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo ( ) { bEnable = true; }
	FORCEINLINE void DisableCombo ( ) { bEnable = false; }
	void AddComboArray ( FString NewCombo );
	bool IsLeftMajority ( );

public:
	void DoAction ( ) override;
	void Begin_DoAction ( ) override;
	void End_DoAction ( ) override;

	void DoHeavyAction ( ) override;
public:
	void OnAttachmentBeginOverlap ( class ACharacter* InAttacker , AActor* InAttackCuaser , class ACharacter* InOther ) override;

private:
	int32 Index=0;
	int32 HeavyIndex = 0;
	int32 DamageIndex;

	bool bEnable;
	bool bExist;

	TArray<FString> ComboArray;

};
