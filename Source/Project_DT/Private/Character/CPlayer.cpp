// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "Animation/AnimInstance.h"
#include "Component/CMointageComponent.h"
#include "Component/CMovementComponent.h"
#include "Component/CStateComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "Component/CTrajectoryComponent.h"
#include "Character/CPlayerAnim.h"
#include "Weapons/CWeaponComponent.h"
// Sets default values
ACPlayer::ACPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USpringArmComponent> ( this , &SpringArm , "SpringArm" , GetMesh ( ) );
	CHelpers::CreateComponent<UCameraComponent> ( this , &Camera , "Camera" , SpringArm );
	GetMesh ( )->SetRelativeLocation ( FVector ( 0 , 0 , -90 ) );
	GetMesh ( )->SetRelativeRotation ( FRotator( 0 , -90 , 0 ) );
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh> ( &mesh , AssetPaths::CHARACTER_MESH );
	GetMesh ( )->SetSkeletalMesh ( mesh );

	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance> ( &Anim , AssetPaths::CHARACTER_ANIM );
	GetMesh ( )->SetAnimClass ( Anim );

	SpringArm->SetRelativeLocation ( FVector ( 0 , 0 , 140 ) );
	SpringArm->SetRelativeRotation ( FRotator ( 0 , 90,0 ) );
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement ( )->RotationRate = FRotator ( ( 0 , 720 , 0 ) );

	CHelpers::CreateActorComponent<UCWeaponComponent> ( this , &Weapon , "Weapon" );
	CHelpers::CreateActorComponent<UCMointageComponent> ( this , &Montages , "Montages" );
	CHelpers::CreateActorComponent<UCMovementComponent> ( this , &Movement , "Movement" );
	CHelpers::CreateActorComponent<UCStateComponent> ( this , &State , "State" );
	CHelpers::CreateActorComponent<UCTrajectoryComponent> ( this , &Trajectory , "Trajectory" );

	//인풋 받기
	CHelpers::GetAsset ( &IMC , AssetPaths::IMC );
	CHelpers::GetAsset ( &IA_MoveForward , AssetPaths::IA_MoveForward );
	CHelpers::GetAsset ( &IA_MoveRight , AssetPaths::IA_MoveRight );
	CHelpers::GetAsset ( &IA_VerticalLook , AssetPaths::IA_VerticalLook );
	CHelpers::GetAsset ( &IA_HorizontalLook , AssetPaths::IA_HorizontalLook );
	CHelpers::GetAsset ( &IA_Dash , AssetPaths::IA_Dash );
	CHelpers::GetAsset ( &IA_Avoid , AssetPaths::IA_Avoid );
	CHelpers::GetAsset ( &IA_TestBtn , AssetPaths::IA_Test );
	CHelpers::GetAsset ( &IA_Jump , AssetPaths::IA_Jump );
	CHelpers::GetAsset ( &IA_LeftAttack , AssetPaths::IA_LeftClick );
	CHelpers::GetAsset ( &IA_RightAttack , AssetPaths::IA_RightClick );
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun ( );
	//Movement->DisableControlRotation ( );
	APlayerController* PC = Cast<APlayerController> ( GetController ( ) );
	UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( PC->GetLocalPlayer ( ) );
	subSys->AddMappingContext ( IMC , 0 );

	State->OnStateTypeChanged.AddDynamic ( this , &ACPlayer::OnStateTypeChanged );
}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto playerInput = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	if ( playerInput ) {
		playerInput->BindAction ( IA_MoveForward , ETriggerEvent::Triggered , Movement , &UCMovementComponent::OnMoveForward );
		playerInput->BindAction ( IA_MoveRight , ETriggerEvent::Triggered , Movement , &UCMovementComponent::OnMoveRight );
		playerInput->BindAction ( IA_VerticalLook , ETriggerEvent::Triggered , Movement , &UCMovementComponent::OnVerticalLook );
		playerInput->BindAction ( IA_HorizontalLook , ETriggerEvent::Triggered , Movement , &UCMovementComponent::OnHorizontalLook );
		playerInput->BindAction ( IA_Dash , ETriggerEvent::Started , Movement , &UCMovementComponent::OnSprint);
		playerInput->BindAction ( IA_Dash , ETriggerEvent::Completed , Movement , &UCMovementComponent::OnRun);
		playerInput->BindAction ( IA_Avoid , ETriggerEvent::Completed , this , &ACPlayer::OnAvoid );
		playerInput->BindAction ( IA_TestBtn , ETriggerEvent::Completed , Weapon , &UCWeaponComponent::SetKatanaMode );
		playerInput->BindAction ( IA_Jump , ETriggerEvent::Completed , this , &ACPlayer::Jump );
		playerInput->BindAction ( IA_LeftAttack , ETriggerEvent::Started , Weapon , &UCWeaponComponent::DoAction );
		playerInput->BindAction ( IA_RightAttack , ETriggerEvent::Started , Weapon , &UCWeaponComponent::DoHeavyAction );
	}
}

void ACPlayer::OnStateTypeChanged ( EStateType InPrevType , EStateType InNewType )
{
	switch ( InNewType )
	{
	case EStateType::BackStep: BackStep ( ); break;
	}
}

void ACPlayer::OnAvoid ( )
{
	if ( !State->IsIdleMode ( ) ) return;
	if ( !Movement->CanMove ( ) ) return;
		State->SetBackStepMode ();
}

void ACPlayer::BackStep ()
{
	Movement->EnableControlRotation ();

	Montages->PlayBackStepMode ();
}

void ACPlayer::Jump ( ){
	CLog::Log ( "Test" );
	ACharacter::Jump();
}

void ACPlayer::End_BackStep() {
	//Movement->DisableControlRotation ( );

	State->SetIdleMode ( );
}
