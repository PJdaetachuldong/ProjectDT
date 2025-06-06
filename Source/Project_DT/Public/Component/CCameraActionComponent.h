// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCameraActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCCameraActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCCameraActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void CameraActionStart(FVector Location,FRotator Rotation,float Length,float Second);
	void CameraActionEnd();
	void ChangeCameraState(int32 Type);
private:
	void SetSpringArmPosition(FVector Location,FRotator Rotation,float Length);
private:
	UPROPERTY ( VisibleAnywhere )
	class ACharacter* OwnerCharacter;
	
	UPROPERTY ( VisibleAnywhere )
	class USpringArmComponent* MainCameraSpringArm;
	
	UPROPERTY ( VisibleAnywhere )
	class USpringArmComponent* SubCameraSpringArm;
	
	UPROPERTY ( VisibleAnywhere )
	class UCameraComponent* Camera;
	
	UPROPERTY ( VisibleAnywhere )
	class USpringArmComponent* SpringArmR;
	
	UPROPERTY ( VisibleAnywhere )
	class USpringArmComponent* SpringArmL;

};
