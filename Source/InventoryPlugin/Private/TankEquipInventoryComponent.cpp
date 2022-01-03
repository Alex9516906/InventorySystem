// Fill out your copyright notice in the Description page of Project Settings.


#include "TankEquipInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EquipInterface.h"


UTankEquipInventoryComponent::UTankEquipInventoryComponent()
{
	EquipSlots.Add(0, EEquipSlot::Es_Head);
	EquipSlots.Add(1, EEquipSlot::Es_Gun);
	EquipSlots.Add(2, EEquipSlot::Es_Armor);
}

void UTankEquipInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{
	IEquipInterface::Execute_EquipItem(UGameplayStatics::GetPlayerPawn(GetWorld(), 0),EquipSlots.FindChecked(SlotIndex),Item.Id);
	Super::SetItem(SlotIndex, Item);
}

void UTankEquipInventoryComponent::ClearItem(int32 SlotIndex)
{
	IEquipInterface::Execute_UnequipItem(UGameplayStatics::GetPlayerPawn(GetWorld(),0),EquipSlots.FindChecked(SlotIndex),GetItem(SlotIndex)->Id);
	Super::ClearItem(SlotIndex);
}
