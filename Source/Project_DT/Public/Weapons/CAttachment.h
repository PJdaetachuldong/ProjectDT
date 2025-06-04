// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FAttachmentBeginCollision );
DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FAttachmentEndCollision );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams ( FAttachmentBeginOverlap , class ACharacter* , InAttacker , AActor* , InAttackCuaser , class ACharacter* , InOther );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams ( FAttachmentEndOverlap , class ACharacter* , InAttacker , class ACharacter* , InOther );

UCLASS()
class PROJECT_DT_API ACAttachment : public AActor
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	class USceneComponent* Root;

public:
	ACAttachment();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION( BlueprintImplementableEvent )
	void OnBeginEquip();
	UFUNCTION (BlueprintImplementableEvent)
	void OnUnequip ( );


protected:
	UFUNCTION(BlueprintCallable,Category="Attach")
	void AttachTo(FName InSocketName);

public:
	void OnCollisions ( );
	void OffCollisions ( );
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision OnAttachmentEndCollision;

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;
protected:
	UPROPERTY ( BlueprintReadOnly , Category = "Game" )
	TArray<class UShapeComponent*>Collisions;

	UPROPERTY ( BlueprintReadOnly , Category = "Game" )
	class ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UNiagaraSystem* TrailNiagaraSystem;

	class UNiagaraComponent* TrailNiagaraStart;

private:
	bool bCollisionTraceEnabled = false;

	FVector PrevStartLocation;
	FVector PrevEndLocation;
	FVector CurrentStartLocation;
	FVector CurrentEndLocation;


	TArray<FHitResult> HitResults;
	void PerformTriangleTrace(
		const FVector& A,
		const FVector& B,
		const FVector& C,
		TArray<FHitResult>& OutHits);
	FVector BezierCurve(const FVector& P0, const FVector& P1, const FVector& P2, float T);
	bool bInitialized = false;

	class AActor* EnemyActor;
};
