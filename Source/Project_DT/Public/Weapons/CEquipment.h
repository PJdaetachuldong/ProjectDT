// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStuctures.h"
#include "CEquipment.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FEquipmentEquip );
DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FEquipmentBeginEquip );
DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FEquipmentUnequip );

UCLASS()
class PROJECT_DT_API UCEquipment : public UObject
{
	GENERATED_BODY()
public:
	void BeginPlay ( class ACharacter* InOwner , TArray<FEquipmentData>& );

public:
	UFUNCTION ( BlueprintNativeEvent )
	void Equip ();

	UFUNCTION ( BlueprintNativeEvent )
	void Begin_Equip ( );

	UFUNCTION ( BlueprintNativeEvent )
	void End_Equip();

	UFUNCTION ( BlueprintNativeEvent )
	void Unequip();
public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnequip OnEquipmentUnequip;

private:
	class ACharacter* OwnerCharacter;
	TArray<FEquipmentData> Data;

private:
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

private:
	bool bBeginEquip;
	bool bEquipped;

};
