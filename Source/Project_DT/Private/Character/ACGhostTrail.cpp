// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACGhostTrail.h"
#include "Components/PoseableMeshComponent.h"
#include "Global.h"
#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/Character.h"

// Sets default values
AACGhostTrail::AACGhostTrail()
{
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");
}

// Called when the game starts or when spawned
void AACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceConstant* material;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material,AssetPaths::ChostTrail);
	Material = UMaterialInstanceDynamic::Create(material, this);
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exponent", Exponent);

	Owner = Cast<ACharacter>(GetOwner());

	Mesh->SetVisibility(false);
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
	Mesh->SetRelativeScale3D(Scale);

	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++) {
		Mesh->SetMaterial(i, Material);
	}
	FTimerHandle timerDelegate;
	auto timerhandler = [this]() {
		if (Mesh->IsVisible() == false)
			Mesh->ToggleVisibility();
		float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		SetActorLocation(Owner->GetActorLocation() - FVector(ScaleAmount.X, ScaleAmount.Y, height - ScaleAmount.Z));
		SetActorRotation(Owner->GetActorRotation() + FRotator(0, -90, 0));
		Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
	};
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerhandler, Interval, true, StartDelay);
}

void AACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}
