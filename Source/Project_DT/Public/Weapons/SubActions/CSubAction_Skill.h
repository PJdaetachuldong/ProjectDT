// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction_Skill.generated.h"

/**
 *
 */
UCLASS( Abstract )
class PROJECT_DT_API UCSubAction_Skill : public UObject
{
	GENERATED_BODY ( )
public:
	virtual void BeginPlay (
	class ACharacter* InOwner ,
	class ACAttachment* InAttachment ,
	class UCDoAction* InDoAction
	);
public:
	virtual void Pressed ( );
	virtual void Released ( ) {};

	UFUNCTION ( BlueprintNativeEvent )
	void Begin_SubAction_Skill ( );
	virtual void Begin_SubAction_Skill_Implementation ( ) {};

	UFUNCTION ( BlueprintNativeEvent )
	void End_SubAction_Skill ( );
	virtual void End_SubAction_Skill_Implementation ( ) {};
	UFUNCTION ( BlueprintNativeEvent )
	void Tick ( float InDeltaTime );
	virtual void Tick_Implementation ( float InDeltaTime ) {};

protected:
	class ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;

};
