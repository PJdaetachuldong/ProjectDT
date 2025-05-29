// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "ICharacter.h"
#include "Component/CParryComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class PROJECT_DT_API ACPlayer : public ACharacter , public IICharacter
{
	GENERATED_BODY()

private:
	UPROPERTY ( VisibleAnywhere )
	class USpringArmComponent* SpringArm;
	UPROPERTY ( VisibleAnywhere )
	class UCameraComponent* Camera;
private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	class UCMointageComponent* Montages;

	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
	class UCParryComponent* Parry;

	UPROPERTY(VisibleAnywhere)
	class UCTargetingComponent* TargetComp;

	UPROPERTY(VisibleAnywhere)
	class UCStatusComponent* Status;

public:
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;
	UPROPERTY(VisibleAnywhere)
	class UCTrajectoryComponent* Trajectory;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UInputAction* IA_MoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_MoveRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_VerticalLook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_HorizontalLook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_Dash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_Avoid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_TestBtn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_TestBtn2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_Jump;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	class UInputAction* IA_LeftAttack;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	class UInputAction* IA_RightAttack;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	class UInputAction* IA_SpecialAttack;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	class UInputAction* IA_Guard;
	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	class UInputAction* IA_Heal;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* IMC;

public:
	// Sets default values for this character's properties
	ACPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
private:
	void OnAvoid();
private:
	void BackStep();
private:
	void Healing ( );
private:
	void Jump();
private:
	void OnSprint ( );
private:
	void DoGuardActionStart ( );
private:
	UFUNCTION()
	void OnParryDetected(EParryState ParryDirection);

public:
	void End_BackStep() override;




};
