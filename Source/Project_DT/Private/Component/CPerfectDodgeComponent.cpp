#include "Component/CPerfectDodgeComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Component/CMointageComponent.h"
#include "Boss/CBossEnemy.h"
#include "Boss/CBossWeapon.h"
UCPerfectDodgeComponent::UCPerfectDodgeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPerfectDodgeComponent::BeginPlay()
{
	Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    Dodge = CHelpers::GetComponent<UCPerfectDodgeComponent>(OwnerCharacter);
    Montage = CHelpers::GetComponent<UCMointageComponent>(OwnerCharacter);
}


void UCPerfectDodgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsDodgeActive)
	{
		PerformBoxTrace();
	}
}

// 박스 트레이스 실행
void UCPerfectDodgeComponent::PerformBoxTrace()
{
    if (!OwnerCharacter) return;

    // 오너의 위치를 기준으로 박스 트레이스
    FVector Start = OwnerCharacter->GetActorLocation();
    FVector End = Start; // 박스 트레이스는 위치만 필요하므로 Start와 End는 동일

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerCharacter); // 오너 자신은 충돌에서 제외

    // 박스 트레이스 실행
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        Start,
        End,
        FQuat::Identity,
        ECC_Visibility, // 충돌 채널: 필요에 따라 변경 가능
        FCollisionShape::MakeBox(BoxExtent),
        QueryParams
    );

    // 디버그용 박스 그리기 (선택사항)
    DrawDebugBox(GetWorld(), Start, BoxExtent, FColor::Red, false, 0.1f);

    // 충돌이 감지되면 방향에 따라 몽타주 재생
    if (bHit)
    {
        if (HitResult.GetActor() && HitResult.GetActor()->IsA(ACBossWeapon::StaticClass()))
        {
            // 충돌 지점과 플레이어 위치를 비교해 방향 계산
            FVector HitLocation = HitResult.ImpactPoint;
            FVector PlayerLocation = OwnerCharacter->GetActorLocation();
            FVector PlayerRight = OwnerCharacter->GetActorRightVector(); // 플레이어의 오른쪽 방향 벡터

            // 충돌 지점을 플레이어의 로컬 공간으로 변환
            FVector DirectionToHit = (HitLocation - PlayerLocation).GetSafeNormal();

            // 오른쪽 벡터와 충돌 방향의 내적을 계산
            float DotProduct = FVector::DotProduct(DirectionToHit, PlayerRight);

            // 내적 값에 따라 왼쪽/오른쪽 구분
            if (DotProduct > 0.0f) // 충돌 지점이 플레이어 기준 오른쪽
            {
                PlayDodgeMontage(true); // 오른쪽 회피
            }
            else // 충돌 지점이 플레이어 기준 왼쪽
            {
                PlayDodgeMontage(false); // 왼쪽 회피
            }
        }
    }
}

// 몽타주 재생
void UCPerfectDodgeComponent::PlayDodgeMontage(bool bDodgeRight)
{
    if (!Montage) return;

    if (bDodgeRight)
    {
        Montage->PlayPerfectDodgeL();
    }
    else
    {
        Montage->PlayPerfectDodgeR();
    }
}