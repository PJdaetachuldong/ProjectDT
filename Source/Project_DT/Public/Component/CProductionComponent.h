// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CProductionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCProductionComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UCProductionComponent();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UFUNCTION(BlueprintCallable, Category = "Camera Control")
	void SetCameraLookAtNearestTarget();

	UFUNCTION(BlueprintCallable, Category = "Camera Control")
	void SetViewToCineCameraByIndex(int32 CameraIndex, float BlendTime = 1.0f, EViewTargetBlendFunction BlendFunc = VTBlend_EaseInOut, float BlendExp = 2.0f);

	UFUNCTION(BlueprintCallable, Category = "Camera Control")
	void SetViewToCharacterCamera(float BlendTime = 1.0f, EViewTargetBlendFunction BlendFunc = VTBlend_EaseInOut, float BlendExp = 2.0f);
private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	FRotator TargetControlRotation;
	bool bInterpolatingCamera;      
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraInterpolationSpeed; 

	UPROPERTY()
	class AActor* CurrentLookAtTargetActor;

	UPROPERTY()
	class ACineCameraActor* CurrentCineCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Control", meta = (AllowPrivateAccess = "true"))
	TArray<ACineCameraActor*> CinematicCameras; // 레벨에서 할당할 Cine Camera Actor 배열
};
