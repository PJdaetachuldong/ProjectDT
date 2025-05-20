// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoActionGuard.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_DT_API UCDoActionGuard : public UCDoAction
{
	GENERATED_BODY()

	void DoGuardStarted ( ) override;
	void DoGuardTrriger ( ) override;
	void DoGuardComplete ( ) override;

private:
	bool bEnable;
	bool bExist;

};
