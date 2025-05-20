// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0, BackStep, Equip, Hitted, Dead, Action,Guard, Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode ( ) { return Type == EStateType::Idle; }
	FORCEINLINE bool IsBackstepMode ( ) { return Type == EStateType::BackStep; }
	FORCEINLINE bool IsEquipMode ( ) { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode ( ) { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode ( ) { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode ( ) { return Type == EStateType::Action; }
	FORCEINLINE bool IsGuardMode ( ) { return Type == EStateType::Guard; }

	FORCEINLINE bool IsSubActionMode ( ) { return bInSubActionMode; }

public:
	// Sets default values for this component's properties
	UCStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay ( ) override;

public:
	// Called every frame
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;

public:
	void SetIdleMode ( );
	void SetBackStepMode ( );
	void SetEquipMode ( );
	void SetHittedMode ( );
	void SetDeadMode ( );
	void SetActionMode ( );
	void SetGuardMode ( );

	void OnSubActionMode ( );
	void OffSubActionMode ( );

private:
	void ChangeType ( EStateType InType );

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;

private:
	bool bInSubActionMode;



};
