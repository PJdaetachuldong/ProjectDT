// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStuctures.h"
#include "CDoAction.generated.h"

UCLASS(Abstract,NotBlueprintable)
class PROJECT_DT_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	UCDoAction();

	virtual void BeginPlay (
		class ACAttachment* InAttachment ,
		class UCEquipment* InEquipment ,
		class ACharacter* InOwner ,
		class TArray<FDoActionData>& InDoActionData
	);

public:
	virtual void DoAction ( );
	virtual void Begin_DoAction ( );
	virtual void End_DoAction ( );

protected:
	bool bBeginAction;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;

};
