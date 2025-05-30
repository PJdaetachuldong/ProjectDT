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
#include "Component/CParryComponent.h"
#include "Component/CTargetingComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CPerfectDodgeComponent.h"
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
	SpringArm->TargetArmLength = 300;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement ( )->bOrientRotationToMovement = true; // 이동 방향을 바라보게
	GetCharacterMovement ( )->bUseControllerDesiredRotation = false;

	// 카메라 설정
	SpringArm->bUsePawnControlRotation = true; // SpringArm이 컨트롤러 회전을 따르지 않음
	Camera->bUsePawnControlRotation = false;

	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;


	GetCharacterMovement ( )->bUseControllerDesiredRotation = false;
	//GetCharacterMovement ( )->RotationRate = FRotator ( ( 0 , 720 , 0 ) );

	CHelpers::CreateActorComponent<UCWeaponComponent> ( this , &Weapon , "Weapon" );
	CHelpers::CreateActorComponent<UCMointageComponent> ( this , &Montages , "Montages" );
	CHelpers::CreateActorComponent<UCMovementComponent> ( this , &Movement , "Movement" );
	CHelpers::CreateActorComponent<UCStateComponent> ( this , &State , "State" );
	CHelpers::CreateActorComponent<UCTrajectoryComponent> ( this , &Trajectory , "Trajectory" );
	CHelpers::CreateActorComponent<UCParryComponent> ( this , &Parry , "Parry" );
	CHelpers::CreateActorComponent<UCPerfectDodgeComponent> ( this , &Dodge, "Dodge" );
	CHelpers::CreateActorComponent<UCTargetingComponent> ( this , &TargetComp , "TargetComp" );
	CHelpers::CreateActorComponent<UCStatusComponent> ( this , &Status , "Status" );



	//인풋 받기
	CHelpers::GetAsset ( &IMC , AssetPaths::IMC );
	CHelpers::GetAsset ( &IA_MoveForward , AssetPaths::IA_MoveForward );
	CHelpers::GetAsset ( &IA_MoveRight , AssetPaths::IA_MoveRight );
	CHelpers::GetAsset ( &IA_VerticalLook , AssetPaths::IA_VerticalLook );
	CHelpers::GetAsset ( &IA_HorizontalLook , AssetPaths::IA_HorizontalLook );
	CHelpers::GetAsset ( &IA_Dash , AssetPaths::IA_Dash );
	CHelpers::GetAsset ( &IA_Avoid , AssetPaths::IA_Avoid );
	CHelpers::GetAsset ( &IA_Jump , AssetPaths::IA_Jump );
	CHelpers::GetAsset ( &IA_LeftAttack , AssetPaths::IA_LeftClick );
	CHelpers::GetAsset ( &IA_RightAttack , AssetPaths::IA_RightClick );
	CHelpers::GetAsset ( &IA_SpecialAttack , AssetPaths::IA_SpecialClick );
	CHelpers::GetAsset ( &IA_Guard , AssetPaths::IA_GuardBtn );
	CHelpers::GetAsset ( &IA_Heal , AssetPaths::IA_Heal );

	CHelpers::GetAsset ( &IA_TestBtn , AssetPaths::IA_Test );
	CHelpers::GetAsset ( &IA_TestBtn2 , AssetPaths::IA_Test2 );
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun ( );
	//Movement->EnableControlRotation ( );
	APlayerController* PC = Cast<APlayerController> ( GetController ( ) );
	UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( PC->GetLocalPlayer ( ) );
	subSys->AddMappingContext ( IMC , 0 );

	State->OnStateTypeChanged.AddDynamic ( this , &ACPlayer::OnStateTypeChanged );
	Parry->OnParryDetected.AddDynamic ( this , &ACPlayer::OnParryDetected );
}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage ( -1 , 0.0f , FColor::Green ,
	FString::Printf ( TEXT ( "Health: %.1f | Mana: %.1f" ) , Status->GetHealth() , Status->GetMana()) );
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
		playerInput->BindAction ( IA_Dash , ETriggerEvent::Triggered , Movement , &UCMovementComponent::OnSprint);
		playerInput->BindAction ( IA_Dash , ETriggerEvent::Completed , Movement , &UCMovementComponent::OnRun);
		playerInput->BindAction ( IA_Avoid , ETriggerEvent::Completed , this , &ACPlayer::OnAvoid );
		//playerInput->BindAction ( IA_Avoid , ETriggerEvent::Completed , TargetComp , &UCTargetingComponent::OnLookOn );
		//playerInput->BindAction ( IA_Jump , ETriggerEvent::Completed , this , &ACPlayer::Jump );
		playerInput->BindAction ( IA_LeftAttack , ETriggerEvent::Started , Weapon , &UCWeaponComponent::DoAction );
		playerInput->BindAction ( IA_RightAttack , ETriggerEvent::Started , Weapon , &UCWeaponComponent::DoHeavyAction );
		playerInput->BindAction ( IA_SpecialAttack , ETriggerEvent::Started , Weapon , &UCWeaponComponent::SubAction_Skill_Pressed );
		playerInput->BindAction ( IA_SpecialAttack , ETriggerEvent::Completed , Weapon , &UCWeaponComponent::SubAction_Skill_Released );
		playerInput->BindAction ( IA_Guard , ETriggerEvent::Started , Weapon , &UCWeaponComponent::SubAction_Pressed);
		playerInput->BindAction ( IA_Guard , ETriggerEvent::Completed , Weapon , &UCWeaponComponent::SubAction_Released);

		playerInput->BindAction ( IA_TestBtn , ETriggerEvent::Completed , Weapon , &UCWeaponComponent::SetKatanaMode );
		playerInput->BindAction ( IA_TestBtn2 , ETriggerEvent::Completed , Weapon , &UCWeaponComponent::SetGreatSwordMode );
		playerInput->BindAction ( IA_Heal , ETriggerEvent::Started , this , &ACPlayer::Healing );
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
	FVector InputDir = GetCharacterMovement ( )->GetLastInputVector ( );

		// 방향을 바라보게 회전
		FRotator NewRotation = InputDir.Rotation ( );
		NewRotation.Pitch = 0;
		NewRotation.Roll = 0;
		SetActorRotation ( NewRotation );

		// 루트 모션 구르기 애니메이션 재생
		Montages->PlayBackStepMode ();
		//Montages->PlayPerfectDodgeL ();

}

void ACPlayer::Healing ( )
{
	Montages->PlayHealingMode ( );
	Status->Damage ( 50 );
	Status->UseMana ( 20 );
}

void ACPlayer::Jump ( ){
	ACharacter::Jump();
}

void ACPlayer::OnParryDetected ( EParryState ParryDirection )
{
	Weapon->OnParry ( ParryDirection );
}

void ACPlayer::End_BackStep() {

	State->SetIdleMode ( );
	CLog::Log ( "End_BackStep");
}