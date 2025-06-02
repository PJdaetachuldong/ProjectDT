// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component/CParryComponent.h"
#include "CWeaponComponent.generated.h"
UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	Fist,Katana,Sword,GreatSword,Max
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
	FORCEINLINE bool IsGreatSwordMode ( ) { return Type == EWeaponType::GreatSword; }
	FORCEINLINE EWeaponType GetWeaponType ( ) { return Type; }
public:
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;

public:
	bool IsIdleMode ( );

public:
	class ACAttachment* GetAttachment ( );
	class UCEquipment* GetEquipment ( );
	class UCDoAction* GetDoAction ( );
	class UCSubAction* GetSubAction ( );
	class UCSubAction_Skill* GetSubAction_Skill ( );
public:
	void SetUnarmedMode ( );
	void SetFistMode ( );
	void SetKatanaMode ( );
	void SetSwordMode ( );
	void SetGreatSwordMode ( );

	void DoAction();
	void DoHeavyAction();
public:
	UFUNCTION(BlueprintCallable)
	void SubAction_Pressed();

	UFUNCTION(BlueprintCallable)
	void SubAction_Released();

	UFUNCTION ( BlueprintCallable )
	void SubAction_Skill_Pressed ( );

	UFUNCTION ( BlueprintCallable )
	void SubAction_Skill_Released ( );
	UFUNCTION ( BlueprintCallable )
	void OnParry ( EParryState ParryState );


private:
	void SetMode ( EWeaponType InType );
	void ChangeType ( EWeaponType InType );
public:
	FWeaponTypeChanged OnWeaponTypeChange;

	bool bIsCombatState = false;
	float CombatStateTime = 0.0f;
private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;

	EWeaponType Type = EWeaponType::Max;

	bool bCanParry = true;



};
