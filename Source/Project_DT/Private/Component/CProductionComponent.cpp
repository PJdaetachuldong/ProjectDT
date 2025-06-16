// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CProductionComponent.h"

#include "Dummy/CTargetActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CinematicCamera/Public/CineCameraActor.h"

UCProductionComponent::UCProductionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    CameraInterpolationSpeed = 5.0f;
    bInterpolatingCamera = false;
}

void UCProductionComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("UCProductionComponent must be attached to an ACharacter."));
        return;
    }

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACineCameraActor::StaticClass(), FoundActors);

    TMap<int32, ACineCameraActor*> SortedCamerasMap;

    for (AActor* FoundActor : FoundActors)
    {
        ACineCameraActor* CineCamera = Cast<ACineCameraActor>(FoundActor);
        if (CineCamera)
        {
            FString CameraName = CineCamera->GetName();
            
            // --- 수정된 부분: 이름 파싱 로직 개선 ---
            // 예시: "BP_Camera1_C_1" -> "Camera1" 부분에서 '1' 추출
            // "BP_Camera" 또는 "Camera" 프리픽스로 시작하는 것을 찾고, 그 뒤의 숫자를 추출
            
            FString BaseName = CameraName;
            int32 CameraNumber = -1; // 유효하지 않은 기본값

            // "BP_Camera" 프리픽스 제거
            if (BaseName.StartsWith("BP_Camera"))
            {
                BaseName.RemoveFromStart("BP_Camera");
            }
            // "Camera" 프리픽스 제거 (BP_Camera가 아닌 일반 Camera 액터도 고려)
            else if (BaseName.StartsWith("Camera"))
            {
                BaseName.RemoveFromStart("Camera");
            }
            else // 우리가 원하는 패턴이 아니면 스킵
            {
                UE_LOG(LogTemp, Warning, TEXT("UCProductionComponent: Cine Camera '%s' does not start with 'BP_Camera' or 'Camera'. It will be ignored."), *CameraName);
                continue; // 다음 카메라로 넘어감
            }

            // 이제 BaseName은 "1_C_1" 또는 "_5_C_0" 같은 형태일 것
            // 여기서 숫자 부분을 찾습니다. 가장 뒤에 오는 숫자를 우선적으로 찾도록 개선
            
            // Regex를 사용하는 것이 가장 견고하지만, 간단한 방법으로도 시도
            // (1) 뒤에서부터 숫자를 찾거나
            // (2) _C_N 패턴을 먼저 제거하거나

            // 방법 1: "_C_X" 패턴이 있다면 제거 (일반적인 언리얼 이름 패턴)
            int32 LastCIndex;
            if (BaseName.FindLastChar(TCHAR('_'), LastCIndex))
            {
                if (LastCIndex > 0 && LastCIndex < BaseName.Len() - 1 && BaseName[LastCIndex - 1] == 'C' && BaseName.Len() > LastCIndex + 1)
                {
                    // "C_N" 부분을 찾아서 제거 시도 (예: "1_C_1" -> "1")
                    BaseName = BaseName.Left(LastCIndex - 1); 
                }
            }

            // 이제 BaseName은 "1" 또는 "_5" 같은 형태가 될 것
            // 숫자만 남은 문자열에서 언더스코어 제거
            BaseName.ReplaceInline(TEXT("_"), TEXT(""), ESearchCase::CaseSensitive);

            if (BaseName.IsNumeric())
            {
                CameraNumber = FCString::Atoi(*BaseName);
            }
            // --- 수정된 부분 끝 ---

            if (CameraNumber >= 0) // 유효한 숫자로 파싱되었는지 확인
            {
                SortedCamerasMap.Add(CameraNumber, CineCamera);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("UCProductionComponent: Failed to parse valid camera number from name: %s (Processed: '%s'). Expected format 'BP_CameraN' or 'BP_Camera_N'."), *CameraName, *BaseName);
            }
        }
    }

    TArray<int32> SortedKeys;
    SortedCamerasMap.GetKeys(SortedKeys);
    SortedKeys.Sort(); 

    CinematicCameras.Empty(); 

    for (int32 Key : SortedKeys)
    {
        CinematicCameras.Add(SortedCamerasMap[Key]); 
        UE_LOG(LogTemp, Log, TEXT("UCProductionComponent: Added CineCamera '%s' to index %d (Original Key: %d)"), *CinematicCameras.Last()->GetName(), CinematicCameras.Num() - 1, Key);
    }

    if (CinematicCameras.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("UCProductionComponent: No CineCameraActors matching 'BP_CameraN' or 'CameraN' pattern found in the level."));
    }
}

void UCProductionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bInterpolatingCamera && OwnerCharacter)
    {
        APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
        if (PlayerController)
        {
            FRotator CurrentControlRotation = PlayerController->GetControlRotation();
            FRotator NewControlRotation = FMath::RInterpTo(CurrentControlRotation, TargetControlRotation, DeltaTime, CameraInterpolationSpeed);

            PlayerController->SetControlRotation(NewControlRotation);

            if (CurrentControlRotation.Equals(TargetControlRotation, 0.5f))
            {
                bInterpolatingCamera = false;
                UE_LOG(LogTemp, Log, TEXT("UCProductionComponent: Camera interpolation finished."));
            }
        }
    }
}

// 매개변수 CameraCurrentLocation을 제거하고, 내부에서 OwnerCharacter의 위치를 사용합니다.
void UCProductionComponent::SetCameraLookAtNearestTarget()
{
    // OwnerCharacter가 유효한지 확인합니다.
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("UCProductionComponent: OwnerCharacter is null. Cannot set camera target."));
        return;
    }

    // 플레이어 컨트롤러를 가져옵니다.
    APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("UCProductionComponent: PlayerController is null. Cannot set camera target."));
        return;
    }

    // 현재 카메라의 시작 위치를 OwnerCharacter의 위치로 설정합니다.
    FVector CameraStartLocation = OwnerCharacter->GetActorLocation();

    // 월드에 있는 모든 ACTargetActor를 찾습니다.
    TArray<AActor*> FoundTargetActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTargetActor::StaticClass(), FoundTargetActors);

    // 가장 가까운 타겟을 찾기 위한 변수 초기화
    AActor* NearestTargetActor = nullptr;
    float MinDistanceSq = MAX_FLT; // 제곱 거리를 사용하여 비교

    for (AActor* TargetActor : FoundTargetActors)
    {
        if (TargetActor)
        {
            float DistanceSq = FVector::DistSquared(CameraStartLocation, TargetActor->GetActorLocation());
            if (DistanceSq < MinDistanceSq)
            {
                MinDistanceSq = DistanceSq;
                NearestTargetActor = TargetActor;
            }
        }
    }

    if (NearestTargetActor)
    {
        CurrentLookAtTargetActor = NearestTargetActor;
        FVector CameraTargetLocation = CurrentLookAtTargetActor->GetActorLocation();

        // 목표 회전만 계산하여 저장합니다.
        TargetControlRotation = UKismetMathLibrary::FindLookAtRotation(CameraStartLocation, CameraTargetLocation);
        
        // 카메라 보간을 시작하도록 플래그를 설정합니다.
        bInterpolatingCamera = true;

        UE_LOG(LogTemp, Log, TEXT("UCProductionComponent: Camera target set to nearest actor: %s. Starting interpolation."), *CurrentLookAtTargetActor->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UCProductionComponent: No ACTargetActor found in the world to look at."));
        // 타겟을 찾지 못했으니 보간을 시작할 수 없습니다.
        bInterpolatingCamera = false; 
    }
}

void UCProductionComponent::SetViewToCineCameraByIndex(int32 CameraIndex, float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp)
{
    if (!OwnerCharacter) return;

    APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
    if (!PlayerController) return;

    // 배열 인덱스 유효성 검사
    if (!CinematicCameras.IsValidIndex(CameraIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("UCProductionComponent: Invalid CameraIndex (%d). CinematicCameras array has %d elements."), CameraIndex, CinematicCameras.Num());
        return;
    }

    ACineCameraActor* TargetCineCamera = CinematicCameras[CameraIndex];

    if (TargetCineCamera)
    {
        bInterpolatingCamera = false; // 보간 중인 카메라 회전 중단

        PlayerController->SetViewTargetWithBlend(TargetCineCamera, BlendTime, BlendFunc, BlendExp, true);
        CurrentCineCamera = TargetCineCamera;
        
        UE_LOG(LogTemp, Log, TEXT("UCProductionComponent: Switched view to Cine Camera at index %d: %s"), CameraIndex, *TargetCineCamera->GetName());
    }
    else
    {
        // 유효하지 않은 인덱스는 아니지만, 해당 인덱스의 액터가 nullptr인 경우 (에디터에서 비어있을 때)
        UE_LOG(LogTemp, Warning, TEXT("UCProductionComponent: Cine Camera at index %d is null. Cannot switch view."), CameraIndex);
    }
}

// 캐릭터 카메라로 다시 전환
void UCProductionComponent::SetViewToCharacterCamera(float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp)
{
    if (!OwnerCharacter) return;

    APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
    if (!PlayerController) return;

    PlayerController->SetViewTargetWithBlend(OwnerCharacter, BlendTime, BlendFunc, BlendExp, true);
    CurrentCineCamera = nullptr; // CineCamera 참조 해제
    SetCameraLookAtNearestTarget();

}