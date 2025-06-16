#include "Widget/CGameOverWidget.h"
#include "Global.h"
#include "Component/CMointageComponent.h"
#include "GameFramework/Character.h"

void UCGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OwnerCharacter=Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	GameOverUIEnd.BindDynamic(this,&UCGameOverWidget::EndGameOverAnimation);
	
	BindToAnimationStarted(GameOverPadeIn, GameOverUIEnd);
}

void UCGameOverWidget::PlayGameOverAnimation()
{
	PlayAnimation(GameOverPadeIn);
}

void UCGameOverWidget::EndGameOverAnimation()
{
	GetWorld()->GetTimerManager().SetTimer(Handler,[this]()
	{
		UCMointageComponent* Montage= CHelpers::GetComponent<UCMointageComponent>(OwnerCharacter);
		Montage->Respawn();	
	},7.0f,false,7.0f);

	
}
