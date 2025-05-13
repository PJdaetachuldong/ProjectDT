// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class PROJECT_DT_API IICharacter
{
	GENERATED_BODY()

public:
	virtual void End_BackStep ( ) {};
};
