// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayer.h"
#include "Global.h"
#include "LHW_GameModeBase.h"
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
#include "Weapons/CWeaponStuctures.h"
#include "Enemy/EnemyBase/CEnemyBase.h"
#include "Boss/CBossWeapon.h"
#include "Component/CProductionComponent.h"
#include "Weapons/CDoAction.h"
#include "Components/WidgetComponent.h"
#include "Widget/CPlayerWidget.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArmL, "SpringArmL", GetMesh());
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArmR, "SpringArmR", GetMesh());
	CHelpers::CreateComponent<USpringArmComponent>(this, &CameraActionArm, "CameraActionArm",GetMesh());
	
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, AssetPaths::CHARACTER_MESH);
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::CHARACTER_ANIM);
	GetMesh()->SetAnimClass(Anim);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	
	SpringArmL->SetRelativeLocation(FVector(60, 0, 140));
	SpringArmL->SetRelativeRotation(FRotator(0, 90, 0));
	
	SpringArmR->SetRelativeLocation(FVector(-60, 0, 140));
	SpringArmR->SetRelativeRotation(FRotator(0, 90, 0));
	
	SpringArm->TargetArmLength = 250;
	SpringArmL->TargetArmLength = 250;
	SpringArmR->TargetArmLength = 250;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;

	SpringArmL->bDoCollisionTest = false;
	SpringArmL->bEnableCameraLag = true;

	SpringArmR->bDoCollisionTest = false;
	SpringArmR->bEnableCameraLag = true;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = false;

	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;

	SpringArmL->bInheritPitch = true;
	SpringArmL->bInheritYaw = true;
	SpringArmL->bInheritRoll = false;

	SpringArmR->bInheritPitch = true;
	SpringArmR->bInheritYaw = true;
	SpringArmR->bInheritRoll = false;


	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMointageComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCTrajectoryComponent>(this, &Trajectory, "Trajectory");
	CHelpers::CreateActorComponent<UCParryComponent>(this, &Parry, "Parry");
	CHelpers::CreateActorComponent<UCPerfectDodgeComponent>(this, &Dodge, "Dodge");
	CHelpers::CreateActorComponent<UCTargetingComponent>(this, &TargetComp, "TargetComp");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCCameraActionComponent>(this, &CameraAction, "CameraAction");
	CHelpers::CreateActorComponent<UMotionWarpingComponent>(this, &MotionWarping, "MotionWarping");
	CHelpers::CreateActorComponent<UCProductionComponent>(this, &Production, "Production");
	

	//인풋 받기
	CHelpers::GetAsset(&IMC, AssetPaths::IMC);
	CHelpers::GetAsset(&IA_MoveForward, AssetPaths::IA_MoveForward);
	CHelpers::GetAsset(&IA_MoveRight, AssetPaths::IA_MoveRight);
	CHelpers::GetAsset(&IA_VerticalLook, AssetPaths::IA_VerticalLook);
	CHelpers::GetAsset(&IA_HorizontalLook, AssetPaths::IA_HorizontalLook);
	CHelpers::GetAsset(&IA_Dash, AssetPaths::IA_Dash);
	CHelpers::GetAsset(&IA_Avoid, AssetPaths::IA_Avoid);
	CHelpers::GetAsset(&IA_Jump, AssetPaths::IA_Jump);
	CHelpers::GetAsset(&IA_LeftAttack, AssetPaths::IA_LeftClick);
	CHelpers::GetAsset(&IA_RightAttack, AssetPaths::IA_RightClick);
	CHelpers::GetAsset(&IA_SpecialAttack, AssetPaths::IA_SpecialClick);
	CHelpers::GetAsset(&IA_Guard, AssetPaths::IA_GuardBtn);
	CHelpers::GetAsset(&IA_Heal, AssetPaths::IA_Heal);

	CHelpers::GetAsset(&IA_TestBtn, AssetPaths::IA_Test);
	CHelpers::GetAsset(&IA_TestBtn2, AssetPaths::IA_Test2);
	CHelpers::GetAsset(&IA_Select, AssetPaths::Select);
	CHelpers::GetAsset(&IA_Cheat, AssetPaths::IA_Cheat);
	CHelpers::GetAsset(&IA_Cheat2, AssetPaths::IA_Cheat2);
	CHelpers::GetAsset(&IA_ESC, AssetPaths::IA_ESC);
	//위젯
	CHelpers::GetClass(&WidgetClass, AssetPaths::PlayerWidget);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun();
	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PC->GetLocalPlayer());
	subSys->AddMappingContext(IMC, 0);
	if (WidgetClass)
	{
		UWidget = CreateWidget<UCPlayerWidget>(GetWorld(), WidgetClass);
	}
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
}

void ACPlayer::Tick(float DeltaTime)
{ 
	Super::Tick(DeltaTime);

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playerInput)
	{
		playerInput->BindAction(IA_MoveForward, ETriggerEvent::Triggered, Movement,
		                        &UCMovementComponent::OnMoveForward);
		playerInput->BindAction(IA_MoveRight, ETriggerEvent::Triggered, Movement, &UCMovementComponent::OnMoveRight);
		playerInput->BindAction(IA_VerticalLook, ETriggerEvent::Triggered, Movement,
		                        &UCMovementComponent::OnVerticalLook);
		playerInput->BindAction(IA_HorizontalLook, ETriggerEvent::Triggered, Movement,
		                        &UCMovementComponent::OnHorizontalLook);
		playerInput->BindAction(IA_Dash, ETriggerEvent::Triggered, Movement, &UCMovementComponent::OnSprint);
		playerInput->BindAction(IA_Dash, ETriggerEvent::Completed, Movement, &UCMovementComponent::OnRun);
		playerInput->BindAction(IA_Avoid, ETriggerEvent::Completed, this, &ACPlayer::OnAvoid);
		playerInput->BindAction(IA_LeftAttack, ETriggerEvent::Started, Weapon, &UCWeaponComponent::DoAction);
		playerInput->BindAction(IA_RightAttack, ETriggerEvent::Started, Weapon, &UCWeaponComponent::DoHeavyAction);
		playerInput->BindAction(IA_SpecialAttack, ETriggerEvent::Started, Weapon,
		                        &UCWeaponComponent::SubAction_Skill_Pressed);
		playerInput->BindAction(IA_SpecialAttack, ETriggerEvent::Completed, Weapon,
		                        &UCWeaponComponent::SubAction_Skill_Released);
		playerInput->BindAction(IA_Guard, ETriggerEvent::Started, this, &ACPlayer::OnGuard);
		playerInput->BindAction(IA_Guard, ETriggerEvent::Completed, this, &ACPlayer::OffGuard);
		playerInput->BindAction(IA_Heal, ETriggerEvent::Started, this, &ACPlayer::Healing);
		

		playerInput->BindAction(IA_TestBtn, ETriggerEvent::Started, this, &ACPlayer::SelectWidgetOn);
		playerInput->BindAction(IA_TestBtn, ETriggerEvent::Completed, this, &ACPlayer::SelectKatana);
		
		playerInput->BindAction(IA_TestBtn2, ETriggerEvent::Started, this, &ACPlayer::SelectWidgetOn);
		playerInput->BindAction(IA_TestBtn2, ETriggerEvent::Completed, this, &ACPlayer::SelectGreatSword);
		playerInput->BindAction(IA_Cheat, ETriggerEvent::Started, Montages, &UCMointageComponent::PlayDeadMode);
		playerInput->BindAction(IA_Cheat2, ETriggerEvent::Started, this, &ACPlayer::TestHandler);
		playerInput->BindAction(IA_Select, ETriggerEvent::Started, this, &ACPlayer::TestHandler2);
		playerInput->BindAction(IA_ESC, ETriggerEvent::Started, this, &ACPlayer::EscapeHandler);
	}
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::BackStep: BackStep();
		break;
	case EStateType::Dead: DeadHandler();
		break;
	}
}

void ACPlayer::OnAvoid()
{
	if (State->IsIdleMode() or State->IsCancelMode())
		State->SetBackStepMode();
}

void ACPlayer::BackStep()
{
	FVector InputDir = GetCharacterMovement()->GetLastInputVector();
	InputDir.Z = 0;
	InputDir.Normalize();

	// 현재 캐릭터의 바라보는 방향 기준으로 입력 벡터를 로컬 방향으로 변환
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	float ForwardDot = FVector::DotProduct(Forward, InputDir);
	float RightDot = FVector::DotProduct(Right, InputDir);

	// 라디안으로 방향 결정
	float Angle = FMath::Atan2(RightDot, ForwardDot);
	float Degree = FMath::RadiansToDegrees(Angle);

	EActState DodgeDirection = EActState::DodgeB; 
	if (!TargetComp->IsLockedOn())
	{
		DodgeDirection = EActState::DodgeF;
		Dodge->DodgeRotate = TEXT("Back");
	}
	else if (Degree==0)
	{
		DodgeDirection = EActState::DodgeF;
		Dodge->DodgeRotate = TEXT("Back");
	}
	else if (Degree >= -22.5f && Degree < 22.5f)
	{
		DodgeDirection = EActState::DodgeF;
		Dodge->DodgeRotate = TEXT("Back");
	}
	else if (Degree >= 22.5f && Degree < 67.5f)
	{
		DodgeDirection = EActState::DodgeFR;
		Dodge->DodgeRotate = TEXT("Right");
	}
	else if (Degree >= 67.5f && Degree < 112.5f)
	{
		DodgeDirection = EActState::DodgeR;
		Dodge->DodgeRotate = TEXT("Right");
	}
	else if (Degree >= 112.5f && Degree < 157.5f)
	{
		DodgeDirection = EActState::DodgeBR;
		Dodge->DodgeRotate = TEXT("Right");
	}
	else if (Degree >= 157.5f || Degree < -157.5f)
	{
		DodgeDirection = EActState::DodgeB;
		Dodge->DodgeRotate = TEXT("Back");
	}
	else if (Degree >= -157.5f && Degree < -112.5f)
	{
		DodgeDirection = EActState::DodgeBL;
		Dodge->DodgeRotate = TEXT("Left");
	}
	else if (Degree >= -112.5f && Degree < -67.5f)
	{
		DodgeDirection = EActState::DodgeL;
		Dodge->DodgeRotate = TEXT("Left");
	}
	else if (Degree >= -67.5f && Degree < -22.5f)
	{
		Dodge->DodgeRotate = TEXT("Left");
		DodgeDirection = EActState::DodgeFL;
	}

	FRotator NewRotation = InputDir.Rotation();
	NewRotation.Pitch = 0;
	NewRotation.Roll = 0;
	SetActorRotation(NewRotation);

	Montages->PlayBackStepMode(DodgeDirection);
}

void ACPlayer::DeadHandler()
{
	Montages->PlayDeadMode();
}

void ACPlayer::Healing()
{
	CheckFalse(State->IsIdleMode());
	if (Status->GetMana()<40)return;
	Montages->PlayHealingMode();

}

void ACPlayer::Jump()
{
	ACharacter::Jump();
}

void ACPlayer::OnParryDetected(EParryState ParryDirection)
{
	Weapon->OnParry(ParryDirection);
}

void ACPlayer::TestHandler()
{
	State->SetStartMode();
	Montages->PlayIntro();
	ALHW_GameModeBase* GameMode=Cast<ALHW_GameModeBase>(GetWorld()->GetAuthGameMode());
	Production->SetViewToCineCameraByIndex(0,1.5f);
}

void ACPlayer::WidgetVisibleHandler(bool isTrue)
{
	if (isTrue)
		UWidget->SetRenderOpacity(0);
	else
		UWidget->SetRenderOpacity(1);
}

void ACPlayer::TestHandler2()
{
	Production->SetViewToCharacterCamera(1.0f);
}

void ACPlayer::EscapeHandler()
{
	ALHW_GameModeBase* Gamemode=Cast<ALHW_GameModeBase>(GetWorld()->GetAuthGameMode());
	CheckNull(Gamemode);
	Gamemode->SwitchOption();
}

void ACPlayer::OnGuard()
{
	Parry->OnParry();
}

void ACPlayer::OffGuard()
{
}

void ACPlayer::End_BackStep()
{
	State->SetIdleMode();
}

void ACPlayer::Hitted()
{
	if (Montages->isDead)return;
	if (Weapon->GetDoAction())
		if (Weapon->GetDoAction()->RetrunParry())return;
	
	if (Status->Damage(Damage.Power) <= 0)
	{
		State->SetDeadMode();
		return;
	}
	Damage.Power = 0;

	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;
		State->SetHittedMode();
		data->PlayMontage(this);
		data->PlayHitStop(GetWorld());
		{
			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

			LaunchCharacter(-direction * data->Launch, false, false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		}
	}
	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}
float ACPlayer::TakeDamage(float TakeDamageAmount, struct FDamageEvent const& DamageEvent,
                           class AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(TakeDamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;
	if (Dodge->ReturnPerfectDodge())return 0;
	Hitted();

	return TakeDamageAmount;
}

void ACPlayer::SelectWidgetOn()
{
	CheckNull(UWidget);
	if (!UWidget->AllowChange)return;
	CheckFalse(State->IsIdleMode());
	UWidget->FadeInSelectWindow();
	UGameplayStatics::SetGlobalTimeDilation(GetOwner(), 0.2f);
}

void ACPlayer::SelectWidgetOff()
{
	CheckNull(UWidget);
	if (!UWidget->AllowChange)return;
	if (UWidget->GetIsCancelWidget())return;
	UGameplayStatics::SetGlobalTimeDilation(GetOwner(), 1.0f);
}

void ACPlayer::SelectKatana()
{
	CheckNull(UWidget);
	if (!UWidget->AllowChange)return;
	if(Weapon->GetWeaponType()==EWeaponType::Katana)
		UWidget->FadeOutSelectWindow();
		
	SelectWidgetOff();
	UWidget->SelectKatana();
}

void ACPlayer::SelectGreatSword()
{
	CheckNull(UWidget);
	if (!UWidget->AllowChange)return;
	if(Weapon->GetWeaponType()==EWeaponType::GreatSword)
		UWidget->FadeOutSelectWindow();
	SelectWidgetOff();
	UWidget->SelectGreatSword();
}
 