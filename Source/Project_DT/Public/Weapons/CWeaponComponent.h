// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"
UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	Fist,Katana,Sword,Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams ( FWeaponTypeChanged , EWeaponType , InPrevType , EWeaponType , InNewType );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="DataAsset")
	class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

public:
	FORCEINLINE bool IsUnarmedMode ( ) { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsFistMode ( ) { return Type == EWeaponType::Fist; }
	FORCEINLINE bool IsKatanaMode ( ) { return Type == EWeaponType::Katana; }
	FORCEINLINE bool IsSwordMode ( ) { return Type == EWeaponType::Sword; }
public:
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool IsIdleMode ( );

public:
	class ACAttachment* GetAttachment ( );
	class UCEquipment* GetEquipment ( );
	class UCDoAction* GetDoAction ( );
public:
	void SetUnarmedMode ( );
	void SetFistMode ( );
	void SetKatanaMode ( );
	void SetSwordMode ( );

	void DoAction();

private:
	void SetMode ( EWeaponType InType );
	void ChangeType ( EWeaponType InType );
public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class ACharacter* OwnerCharacter;

	EWeaponType Type = EWeaponType::Max;

};
