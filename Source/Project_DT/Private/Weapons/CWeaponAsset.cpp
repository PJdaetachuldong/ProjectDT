// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "GameFramework/Character.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"

UCWeaponAsset::UCWeaponAsset ( )
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
	DoActionClass = UCDoAction::StaticClass();
}

void UCWeaponAsset::BeginPlay ( class ACharacter* InOwner )
{
	if ( !!AttachmentClass ) {
		FActorSpawnParameters parmas;
		parmas.Owner = InOwner;

		Attachment = InOwner->GetWorld ( )->SpawnActor<ACAttachment> ( AttachmentClass , parmas );
	}
	if ( !!EquipmentClass ) {
		Equipment = NewObject<UCEquipment> ( this , EquipmentClass );
		Equipment->BeginPlay ( InOwner , EquipmentData );

		if ( !!Attachment ) {
			Equipment->OnEquipmentBeginEquip.AddDynamic ( Attachment , &ACAttachment::OnBeginEquip );
			Equipment->OnEquipmentUnequip.AddDynamic ( Attachment , &ACAttachment::OnUnequip );
		}
	}
	if ( !!DoActionClass ) {
		DoAction = NewObject<UCDoAction> ( this , DoActionClass );
		DoAction->BeginPlay ( Attachment , Equipment , InOwner , DoActionDatas, DoHeavyActionDatas, DoSpecialActionData ,HitDatas );

		if ( !!Attachment ) {
			Attachment->OnAttachmentBeginCollision.AddDynamic ( DoAction , &UCDoAction::OnAttachmentBeginCollision );
			Attachment->OnAttachmentEndCollision.AddDynamic ( DoAction , &UCDoAction::OnAttachmentEndCollision );
			Attachment->OnAttachmentBeginOverlap.AddDynamic ( DoAction , &UCDoAction::OnAttachmentBeginOverlap );
			Attachment->OnAttachmentEndOverlap.AddDynamic ( DoAction , &UCDoAction::OnAttachmentEndOverlap );
		}

	}
}
