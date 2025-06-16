// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_Intro.h"
#include "LHW_GameModeBase.h"

FString UCAnimNotify_Intro::GetNotifyName_Implementation() const
{
	return "Intro";
}

void UCAnimNotify_Intro::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ALHW_GameModeBase* GameMode = Cast<ALHW_GameModeBase>(MeshComp->GetOwner()->GetWorld());
	GameMode->CreateScriptUI("어라 ? 여기는... 어쩌다 잠이 들었더라 ? 기억이 잘 나지 않아...\n우선은 더 어두워 지기 전에 빨리 집으로 돌아가야겠다.");

}
