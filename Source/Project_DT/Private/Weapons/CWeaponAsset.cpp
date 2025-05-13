// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "GameFramework/Character.h"
#include "Weapons/CEquipment.h"

UCWeaponAsset::UCWeaponAsset ( )
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void UCWeaponAsset::BeginPlay ( class ACharacter* InOwner )
{
	if ( !!AttachmentClass ) {
		FActorSpawnParameters parmas;
		parmas.Owner = InOwner;

		Attachment = InOwner->GetWorld ( )->SpawnActor<ACAttachment> ( AttachmentClass , parmas );
	}
	//if ( !!EquipmentClass ) {
	//	Equipment = NewObject<UCEquipment> ( this , EquipmentClass );
	//	Equipment->BeginPlay ( InOwner , EquipmentData );
	//}
}
