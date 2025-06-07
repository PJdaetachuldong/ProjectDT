// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPerfectDodgeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCPerfectDodgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCPerfectDodgeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
    FORCEINLINE void StartPerfectDodge() { bIsDodgeActive = true; };
    FORCEINLINE void EndPerfectDodge() { bIsDodgeActive = false; };
    FORCEINLINE bool ReturnPerfectDodge() { return bIsDodgeActive; };
private:
    // 퍼펙트 닷지 활성화 여부
    UPROPERTY()
    bool bIsDodgeActive = false;

    // 박스 트레이스 크기
    UPROPERTY(EditAnywhere, Category = "PerfectDodge")
    FVector BoxExtent = FVector(50.0f, 50.0f, 80.0f);
    UPROPERTY()
    class ACharacter* OwnerCharacter;
    UPROPERTY()
    class UCPerfectDodgeComponent* Dodge;
    UPROPERTY()
    class UCMointageComponent* Montage;
    UPROPERTY()
    class UCMovementComponent* Movement;
	

private:
	UPROPERTY()
	TArray<AActor*> TemporarilyIgnoredActors;

    // 박스 트레이스 실행 함수
    void PerformBoxTrace();

    // 몽타주 재생 함수
    void PlayDodgeMontage();
public:
    FString DodgeRotate = TEXT("Back");
};
