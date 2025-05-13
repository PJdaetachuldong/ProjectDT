// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStuctures.generated.h"

USTRUCT()
struct FEquipmentData {
	GENERATED_BODY ( )

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
	float PlayRate =1;
	UPROPERTY(EditAnywhere)
	bool bCanMove =true;
	UPROPERTY(EditAnywhere)
	bool bUseControlRotation=true;
};

UCLASS()
class PROJECT_DT_API UCWeaponStuctures : public UObject
{
	GENERATED_BODY()

};
