// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStuctures.h"
#include "CWeaponAsset.generated.h"

UCLASS()
class PROJECT_DT_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
	TArray< FEquipmentData> EquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FDoHeavyActionData> DoHeavyActionDatas;

	UPROPERTY ( EditAnywhere )
	TArray<FCounterActionData> CounterActionDatas;

	UPROPERTY ( EditAnywhere )
	TArray<FParryActionData> ParryActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas;
	UPROPERTY ( EditAnywhere )
	TSubclassOf<class UCSubAction> SubActionClass;
	UPROPERTY ( EditAnywhere )
	TSubclassOf<class UCSubAction_Skill> SubAction_SkillClass;

public:
	FORCEINLINE class ACAttachment* GetAttachment ( ) { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment ( ) { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction ( ) { return DoAction; }
	FORCEINLINE class UCSubAction* GetSubAction ( ) { return SubAction; }
	FORCEINLINE class UCSubAction_Skill* GetSubAction_Skill ( ) { return SubAction_Skill; }

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

	UPROPERTY()
	class ACAttachment* Attachment;

	UPROPERTY()
	class UCEquipment* Equipment;

	UPROPERTY()
	class UCDoAction* DoAction;

	UPROPERTY ( )
	class UCSubAction* SubAction;

	UPROPERTY ( )
	class UCSubAction_Skill* SubAction_Skill;


};
