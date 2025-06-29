// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_HitCamearaShake.h"
#include "Global.h"

FString UCAnimNotify_HitCamearaShake::GetNotifyName_Implementation ( ) const
{
	return "Hit_CameraShake";
}

void UCAnimNotify_HitCamearaShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
    
	// 메시 컴포넌트와 오너(캐릭터)가 유효한지 확인
	CheckNull(MeshComp);
	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	// 카메라 쉐이크 클래스가 유효한지 확인
	if (!CameraShakeClass)
	{
		CLog::Log("CameraShakeClass is not set in Hit_CameraShake Notify!", __FILE__, __LINE__);
		return;
	}

	// 오너 액터 (캐릭터)의 컨트롤러를 가져옴
	AController* Controller = nullptr;
	APawn* OwningPawn = Cast<APawn>(OwnerActor);
	if (OwningPawn)
	{
		Controller = OwningPawn->GetController();
	}
    
	// 컨트롤러가 플레이어 컨트롤러인지 확인
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		// 플레이어 컨트롤러의 카메라에 쉐이크 재생
		PlayerController->ClientStartCameraShake(CameraShakeClass, ShakeScale);
	}
	else
	{
		// 플레이어 컨트롤러가 아닌 경우 (예: NPC가 맞았을 때) 콘솔 로그 출력
		// CLog::Log("Not a Player Controller. Camera shake will not be played.", __FILE__, __LINE__);
	}
}