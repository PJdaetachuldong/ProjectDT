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
	FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;
public:
	FORCEINLINE class ACAttachment* GetAttachment ( ) { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment ( ) { return Equipment; }

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

	UPROPERTY()
	class ACAttachment* Attachment;

	UPROPERTY()
	class UCEquipment* Equipment;


};
