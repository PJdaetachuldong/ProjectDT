#include "Widget/CGameOverWidget.h"

void UCGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// GameOverUIEnd.BindDynamic(this,&UCGameOverWidget::KatanaEndSelect);
	
	BindToAnimationFinished(GameOverPadeIn, GameOverUIEnd);
}
