// AssetPaths.h
#pragma once

#include "CoreMinimal.h"

namespace AssetPaths
{
    //캐릭터 기본 세팅
    constexpr const TCHAR* CHARACTER_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/Asset/GhostSamurai_Bundle/GhostSamurai_Bundle/GhostSamurai/Character/Mesh/SK_GhostSamurai.SK_GhostSamurai'" );
    constexpr const TCHAR* CHARACTER_ANIM = TEXT ( "/Script/Engine.AnimBlueprint'/Game/LHW/Blueprint/ABP_Player.ABP_Player_C'" );
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

    constexpr const TCHAR* IMC = TEXT ( "/Script/EnhancedInput.InputMappingContext'/Game/LHW/Input/IMC_Player.IMC_Player'" );
    constexpr const TCHAR* IA_Test = TEXT ("/Script/EnhancedInput.InputAction'/Game/LHW/Input/IA_Test.IA_Test'" );


}