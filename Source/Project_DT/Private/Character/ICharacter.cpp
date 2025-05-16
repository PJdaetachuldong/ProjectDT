// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ICharacter.h"
#include "GameFramework/Character.h"

void IICharacter::Create_DynamicMeterical ( class ACharacter* InCharacter )
{
	for ( int32 i = 0; i < InCharacter->GetMesh()->GetMaterials().Num(); i++ )
	{
		UMaterialInterface* material = InCharacter->GetMesh ( )->GetMaterials ( )[i];

		InCharacter->GetMesh ( )->SetMaterial ( i , UMaterialInstanceDynamic::Create ( material , InCharacter ) );
	}
}

void IICharacter::Change_Color ( class ACharacter* InCharacter , FLinearColor InColor )
{
	for ( UMaterialInterface* material : InCharacter->GetMesh()->GetMaterials() )
	{
		UMaterialInstanceDynamic* instance = Cast<UMaterialInstanceDynamic> ( material );

		if ( !!instance ) {
			instance->SetDoubleVectorParameterValue ( "BodyColor" , InColor );
		}
	}
}
