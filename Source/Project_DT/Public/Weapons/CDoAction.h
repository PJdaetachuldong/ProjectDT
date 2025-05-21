// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStuctures.h"
#include "CDoAction.generated.h"
UENUM(BlueprintType)
enum class EActionState : uint8 {
	Normal,Heavy, Special, Guard ,Max
};

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
		class TArray<FDoActionData>& InDoActionData,
		class TArray<FDoHeavyActionData>& InDoHeavyActionData,
		class TArray<FHitData>& InHitData
	);

public:
	virtual void DoAction ();
	virtual void Begin_DoAction ( );
	virtual void End_DoAction ( );

	virtual void DoHeavyAction ();
	virtual void Begin_DoHeavyAction ( );

	virtual void ResetDoAction ( ) {};



public:
	FORCEINLINE void NormalAttack ( ) { ActionState = EActionState::Normal; };
	FORCEINLINE void HeavyAttack ( ) { ActionState = EActionState::Heavy; };
	FORCEINLINE void SpecialAttack ( ) { ActionState = EActionState::Special; };
	FORCEINLINE void OnGuard ( ) { ActionState = EActionState::Guard; };

public:
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() { }

	UFUNCTION()
		virtual void OnAttachmentEndCollision() { }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther) { }

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) { }

protected:
	bool bBeginAction;
	EActionState ActionState=EActionState::Max;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	TArray<FDoHeavyActionData> DoHeavyActionDatas;
	TArray<FHitData> HitDatas;

};
