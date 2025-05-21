// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Component/CParryComponent.h"
#include "CSubAction.generated.h"

/**
 *
 */
UCLASS(Abstract)
class PROJECT_DT_API UCSubAction : public UObject
{
	GENERATED_BODY()
public:
	virtual void BeginPlay (
	class ACharacter* InOwner ,
	class ACAttachment* InAttachment ,
	class UCDoAction* InDoAction
	);
public:
	virtual void Pressed ( ) {};
	virtual void Released ( ) {};

	virtual void Parry ( EParryState ParryState ) {};

	UFUNCTION (BlueprintNativeEvent)
	void Begin_SubAction ( );
	virtual void Begin_SubAction_Implementation ( ) {};

	UFUNCTION ( BlueprintNativeEvent )
	void End_SubAction ( );
	virtual void End_SubAction_Implementation ( ) {};
	UFUNCTION ( BlueprintNativeEvent )
	void Tick ( float InDeltaTime );
	virtual void Tick_Implementation ( float InDeltaTime ) {};

protected:
	class ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;

	class UAnimInstance* AnimInstance;
};
