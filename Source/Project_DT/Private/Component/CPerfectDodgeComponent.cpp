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
                PlayDodgeMontage();
        }
    }
}

// 몽타주 재생
void UCPerfectDodgeComponent::PlayDodgeMontage()
{
    if (!Montage) return;

    if (DodgeRotate =="Left")
    {
        Montage->PlayPerfectDodgeL();
    }
    else if(DodgeRotate == "Right")
    {
        Montage->PlayPerfectDodgeR();
    }
    else {
        Montage->PlayPerfectDodge();
    }
}