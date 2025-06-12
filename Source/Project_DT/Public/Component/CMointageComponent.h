// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.h"
#include "CMointageComponent.generated.h"
UENUM ( )
enum class EActState : uint8
{
	Healing,DodgeF, DodgeB , DodgeL, DodgeR, DodgeFL, DodgeFR, DodgeBL, DodgeBR , PDodgeL, PDodgeR,Dead, PDodge, Max
};

USTRUCT()
struct FMontagesData
	: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EActState Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDieDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCMointageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnDieDelegate DieDelegate;
private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* DataTable;
public:
	// Sets default values for this component's properties
	UCMointageComponent ();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	void PlayBackStepMode(EActState InType);
	void PlayDeadMode();
	void PlayEquipMode ( );
	void PlayHealingMode ( );
	void PlayPerfectDodgeL ( );
	void PlayPerfectDodgeR ( );
	void PlayPerfectDodge ( );
	void Dead();
	void Respawn();

private:
	void PlayAnimMontage( EActState InType);

private:
	class ACharacter* OwnerCharacter;
	FMontagesData* Datas[(int32)EActState::Max];
	class UCStateComponent* State;
	class UCStatusComponent* Status;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector RespawnPosition=FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameOverClass;

public:
	UPROPERTY(EditAnywhere)
	class UCGameOverWidget* GameOver;

public:
	bool isDead=false;

};
