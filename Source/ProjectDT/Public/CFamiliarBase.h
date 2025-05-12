// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CFamiliarBase.generated.h"

UCLASS()
class PROJECTDT_API ACFamiliarBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACFamiliarBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:

};
