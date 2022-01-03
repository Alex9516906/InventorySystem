

#include "InventoryComponent.h"


void UInventoryComponent::SpawnInventory()
{
	
	const auto ArrayNameItemsToSpawn = SpawnItemTable->GetRowNames();
	int i=0;
	for(auto NameItem: ArrayNameItemsToSpawn)
	{
		SetItem(i++,*SpawnItemTable->FindRow<FInventorySlotInfo>(NameItem,""));
	}
	SetInit();
	
}

void UInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{
	Items.Add(SlotIndex, Item);
}


void UInventoryComponent::ClearItem(int32 SlotIndex)
{
	Items.Remove(SlotIndex);
}

int32 UInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& InItem)
{
	FInventorySlotInfo* SlotPtr = Items.Find(SlotIndex);
	if(SlotPtr==nullptr || SlotPtr->Id ==InItem.Id)
	{
		return 0;
	}
	return  - 1;
}
