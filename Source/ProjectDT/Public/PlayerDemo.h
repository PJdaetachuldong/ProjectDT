// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StateComponent.h"
#include "PlayerDemo.generated.h"

UCLASS()
class PROJECTDT_API APlayerDemo : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerDemo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;
private:
	UPROPERTY(VisibleAnywhere)
	class UMontageComponent* Montages;
	UPROPERTY(VisibleAnywhere)
	class UMovemontComponent* Movement;
	UPROPERTY(VisibleAnywhere)
	class UStateComponent* State;
	UPROPERTY(EditAnywhere)
	class UInputAction* IA_


	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDemoAnimInstance> AimInstance;
private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
private:
	void OnAvoid();
private:
	void BackStep();



};
