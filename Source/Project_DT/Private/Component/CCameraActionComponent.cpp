

#include "Component/CCameraActionComponent.h"

#include "GameFramework/Character.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


UCCameraActionComponent::UCCameraActionComponent()
{
}
void UCCameraActionComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter=Cast<ACharacter>(GetOwner());
	MainCameraSpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter, "SpringArm");
	SubCameraSpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter, "CameraActionArm");
	SpringArmR = CHelpers::GetComponent<USpringArmComponent> ( OwnerCharacter,"SpringArmR" );
	SpringArmL = CHelpers::GetComponent<USpringArmComponent> ( OwnerCharacter,"SpringArmL" );
    
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
}


void UCCameraActionComponent::CameraActionStart(FVector Location,FRotator Rotation,float Length,float Second)
{
	SetSpringArmPosition(Location,Rotation,Length);
	
	Camera->AttachToComponent(SubCameraSpringArm, FAttachmentTransformRules::KeepWorldTransform);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Camera, FVector::ZeroVector, FRotator::ZeroRotator, true, true, Second, false,    EMoveComponentAction::Type::Move,
	LatentInfo);
}

void UCCameraActionComponent::CameraActionEnd()
{

	Camera->AttachToComponent(MainCameraSpringArm, FAttachmentTransformRules::KeepWorldTransform);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Camera, FVector::ZeroVector, FRotator::ZeroRotator, true, true, 0.4f, false,    EMoveComponentAction::Type::Move,
	LatentInfo);
}

void UCCameraActionComponent::SetSpringArmPosition(FVector Location,FRotator Rotation,float Length)
{
	CheckNull(SubCameraSpringArm);
	SubCameraSpringArm->SetRelativeLocation(Location);
	SubCameraSpringArm->SetRelativeRotation(Rotation);
	SubCameraSpringArm->TargetArmLength=Length;
	
}

void UCCameraActionComponent::ChangeCameraState(int32 Type)
{
	switch (Type)
	{
	case 1:
		{
			Camera->AttachToComponent(MainCameraSpringArm, FAttachmentTransformRules::KeepWorldTransform);
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(Camera, FVector::ZeroVector, FRotator::ZeroRotator, true, true, 0.4f, false,    EMoveComponentAction::Type::Move,
			LatentInfo);
		}break;
	case 2:
		{
			Camera->AttachToComponent(SpringArmR, FAttachmentTransformRules::KeepWorldTransform);
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(Camera, FVector::ZeroVector, FRotator::ZeroRotator, true, true, 0.4f, false,    EMoveComponentAction::Type::Move,
			LatentInfo);
		}break;
	case 3:
		{
			Camera->AttachToComponent(SpringArmL, FAttachmentTransformRules::KeepWorldTransform);
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(Camera, FVector::ZeroVector, FRotator::ZeroRotator, true, true, 0.4f, false,    EMoveComponentAction::Type::Move,
			LatentInfo);
		}break;
	}
}