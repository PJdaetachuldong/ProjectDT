// AssetPaths.h
#pragma once

#include "CoreMinimal.h"

namespace AssetPaths
{
    //캐릭터 기본 세팅
    constexpr const TCHAR* CHARACTER_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/LittleGirlEllie/BaseMesh/Separate/SK_body.SK_body'" );
    constexpr const TCHAR* CHARACTER_ANIM = TEXT ( "/Script/Engine.AnimBlueprint'/Game/LHW/Blueprint/Character/ABP_Ellie.ABP_Ellie_C'" );
    //캐릭터 입력 인풋
    constexpr const TCHAR* IA_MoveForward = TEXT ( "/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_MoveForward.IA_MoveForward'" );
    constexpr const TCHAR* IA_MoveRight = TEXT ( "/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_MoveRight.IA_MoveRight'" );
    constexpr const TCHAR* IA_VerticalLook = TEXT ( "/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_VerticalLook.IA_VerticalLook'" );
    constexpr const TCHAR* IA_HorizontalLook = TEXT ( "/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_HorizontalLook.IA_HorizontalLook'" );
    constexpr const TCHAR* IA_Dash = TEXT ( "/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Dash.IA_Dash'" );
    constexpr const TCHAR* IA_Avoid = TEXT ( "/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Avoid.IA_Avoid'" );
    constexpr const TCHAR* IA_Jump = TEXT ( "/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Jump.IA_Jump'" );
    constexpr const TCHAR* IA_LeftClick = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_LeftClick.IA_LeftClick'" );
    constexpr const TCHAR* IA_RightClick = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_RightClick.IA_RightClick'" );
    constexpr const TCHAR* IA_SpecialClick = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Ctrl.IA_Ctrl'" );
    constexpr const TCHAR* IA_GuardBtn = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Guard.IA_Guard'" );
    constexpr const TCHAR* IA_Heal = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_E.IA_E'" );
    constexpr const TCHAR* IA_Cheat = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Cheat.IA_Cheat'" );
    constexpr const TCHAR* IA_Cheat2 = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Cheat2.IA_Cheat2'" );
    constexpr const TCHAR* IA_ESC = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_ESC.IA_ESC'" );

    constexpr const TCHAR* IMC = TEXT ( "/Script/EnhancedInput.InputMappingContext'/Game/LHW/Input/IMC_Player.IMC_Player'" );
    constexpr const TCHAR* IA_Test = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Test.IA_Test'" );
    constexpr const TCHAR* IA_Test2 = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Test2.IA_Test2'" );
    constexpr const TCHAR* ChostTrail = TEXT ("/Script/Engine.MaterialInstanceConstant'/Game/LHW/Material/M_GhostTrail_Inst.M_GhostTrail_Inst'" );
    constexpr const TCHAR* PlayerWidget = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_PlayerWidget.WBP_PlayerWidget_C'" );
    constexpr const TCHAR* Select = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Select.IA_Select'" );
    
    constexpr const TCHAR* BossUI = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_BossWidget.WBP_BossWidget_C'" );
    constexpr const TCHAR* LoadingUI = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_Loading.WBP_Loading_C'" );
    constexpr const TCHAR* IntroUI = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_Intro.WBP_Intro_C'" );
    constexpr const TCHAR* ScriptUI = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_Script.WBP_Script_C'" );
    constexpr const TCHAR* MapWidget = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_MapWidget.WBP_MapWidget_C'" );
    constexpr const TCHAR* GameOverWidget = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_GameOverWidget.WBP_GameOverWidget_C'" );
    constexpr const TCHAR* QuestWidget = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_Quest.WBP_Quest_C'" );
    constexpr const TCHAR* OptionWidget = TEXT ("/Script/UMGEditor.WidgetBlueprint'/Game/LHW/Widget/WBP_Option.WBP_Option_C'" );

    //보스
    constexpr const TCHAR* BossAnim = TEXT ("/Script/Engine.AnimBlueprint'/Game/ODH/Animation/Boss/ABP_BossAnim.ABP_BossAnim_C'" );
    constexpr const TCHAR* MeleeAnim = TEXT ("/Script/Engine.AnimBlueprint'/Game/ODH/Animation/Enemy/ABP_Enemy.ABP_Enemy_C'" );

    //튜토 에너미
    constexpr const TCHAR* TutoAnim = TEXT("/Script/Engine.AnimBlueprint'/Game/ODH/Animation/Tuto/ABP_Tuto.ABP_Tuto_C'");
}