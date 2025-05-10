// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDemo.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DemoAnimInstance.h"
#include "MontageComponent.h"
#include "MovemontComponent.h"

// Sets default values
APlayerDemo::APlayerDemo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm);
	ConstructorHelpers::FClassFinder<UDemoAnimInstance> a(TEXT("/Script/Engine.AnimBlueprint'/Game/LHW/BluePrint/ABP_Demo.ABP_Demo_C'"));
	GetMesh()->SetAnimClass(a.Class);

}

// Called when the game starts or when spawned
void APlayerDemo::BeginPlay()
{
	Super::BeginPlay();
	Movement->OnRun();
	Movement->DisableControlRotation();

	State->OnStateTypeChanged.AddDynamic(this, &APlayerDemo::OnStateTypeChanged);

}

// Called every frame
void APlayerDemo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerDemo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerDemo::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::BackStep: BackStep(); break;
	}
}

void APlayerDemo::OnAvoid()
{
	if (!State->IsIdleMode()) return;
	if (!Movement->CanMove()) return;
	if (InputComponent->GetAxisValue("MoveForward") >= 0.0f) {


	State->SetBackStepMode();
	}
}

void APlayerDemo::BackStep()
{
	Movement->EnableControlRotation();

	Montages->PlayBackStepMode();
}

