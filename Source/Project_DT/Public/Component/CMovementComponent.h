// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk = 0, Run, Sprint, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float HorizontalLook = 45;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float VerticalLook = 45;

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ESpeedType::Max] = { 200, 200, 600 };

public:
	FORCEINLINE bool CanMove() { return bCanMove; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }

	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

	FORCEINLINE void EnableTopViewCamera() { bTopViewCamera = true; }
	FORCEINLINE void DisableTopViewCamera() { bTopViewCamera = false; }

public:
	// Sets default values for this component's properties
	UCMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	void SetSpeed(ESpeedType InType);

public:
	void OnSprint();
	void OnRun();
	void OnWalk();

	void EnableControlRotation();
	void DisableControlRotation();

public:
	void OnMoveForward(const FInputActionValue& Value);
	void OnMoveRight( const FInputActionValue& Value );
	void OnHorizontalLook( const FInputActionValue& Value );
	void OnVerticalLook( const FInputActionValue& Value );

public:
	void Move();
	void Stop();

private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;

private:
	bool bCanMove = true;
	bool bFixedCamera;
	bool bTopViewCamera;


};
