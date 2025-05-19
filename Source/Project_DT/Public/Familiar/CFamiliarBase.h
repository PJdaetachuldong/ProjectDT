// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CFamiliarBase.generated.h"

UCLASS()
class PROJECT_DT_API ACFamiliarBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACFamiliarBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;



};
