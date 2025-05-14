// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

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

public:
	UFUNCTION( BlueprintImplementableEvent )
	void OnBeginEquip();
	UFUNCTION (BlueprintImplementableEvent)
	void OnUnequip ( );
protected:
	UPROPERTY (BlueprintReadOnly,Category="Game" )
	class ACharacter* OwnerCharacter;

protected:
	UFUNCTION(BlueprintCallable,Category="Attach")
	void AttachTo(FName InSocketName);


};
