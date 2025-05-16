#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CEnemyStats.h"
#include "CEnemyDataAsset.generated.h"

UCLASS()
class PROJECT_DT_API UCEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FCEnemyStats Stats;
};
