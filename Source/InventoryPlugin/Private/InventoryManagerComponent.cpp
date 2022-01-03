
#include "InventoryManagerComponent.h"
#include "ToolContextInterfaces.h"
#include "InventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EquipInterface.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
	if(InInventoryComponent)
	{
		InInventoryComponent->SpawnInventory();
		LocalInventory=InInventoryComponent;
	}
	
	if(LocalInventory && ItemsData)
	{
		ensure(InventoryWidgetClass);
		ensure(MainInventoryWidgetClass);
		MainInventoryWidget =  CreateWidget<UTankInventoryMainWidget>(GetWorld(), 
		   MainInventoryWidgetClass);

		InventoryWidget = MainInventoryWidget->InventoryWidget;
		InventoryWidget->ParentInventory = InInventoryComponent;
		if(!InventoryWidget)
		{
			return;
		}
		InventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);
		InventoryWidget->OnItemUse.AddUObject(this, &ThisClass::OnItemUsed);
		
		InventoryWidget->Init( FMath::Max(LocalInventory->GetItemNum(), MinInventorySize));
		for (auto& Item : LocalInventory->GetItems())
		{
			const FInventoryItemInfo* ItemData = GetItemData(Item.Value.Id);
			if (ItemData)
			{
				ItemData->Icon.LoadSynchronous();
				InventoryWidget->AddItem(Item.Value, *ItemData, Item.Key);
			}
		}
		MainInventoryWidget->AddToViewport();
		MainInventoryWidget->ManagerComponent = this;
	}
}

void UInventoryManagerComponent::EquipInit(UInventoryComponent* InInventoryComponent)
{
	if(InInventoryComponent)
	{
		EquipInventoryWidget = MainInventoryWidget->EquipWidget;
		if(!EquipInventoryWidget)
		{
			return;
		}
		EquipInventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);
		EquipInventoryWidget->ParentInventory = InInventoryComponent;
	}
}
void UInventoryManagerComponent::OnItemUsed(UInventoryCellWidget* UsedItem)
{
	if(GetItemData(UsedItem->GetItem().Id)->Type == EItemType::It_Consumable)
	{
		IEquipInterface::Execute_UseItem(UGameplayStatics::GetPlayerPawn(GetWorld(),0), UsedItem->GetItem().Id);
		SubtractItem(UsedItem);
	}
}

UInventoryCellWidget* UInventoryManagerComponent::FindClearSlot()
{
	for(const auto Widget :InventoryWidget->CellWidgets)
	{
		if( Widget->IsClear())
		{
			return Widget;
		}
	}
	return nullptr;
}


void UInventoryManagerComponent::OpenInventory(const bool bOpen)
{
	if(bOpen) // close
	{
		MainInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else // open
	{
		if(!LocalInventory->IsInit())
		{
			Init(LocalInventory);
			EquipInit(EquipInventory);
		}else
		{
			MainInventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryManagerComponent::SetTableToSpawn(UDataTable* Table)
{
	LocalInventory->SpawnItemTable = Table;
}

void UInventoryManagerComponent::SetLocalInventory(UInventoryComponent* InInventoryComponent)
{
	LocalInventory = InInventoryComponent;
}

void UInventoryManagerComponent::SetLocalEquipInventory(UInventoryComponent* InInventoryComponent)
{
	EquipInventory = Cast<UTankEquipInventoryComponent>(InInventoryComponent);
}

const FInventoryItemInfo* UInventoryManagerComponent::GetItemData(const FName& InId) const
{
	return ItemsData ? ItemsData->FindRow<FInventoryItemInfo>(InId,"" ):nullptr;
}

void UInventoryManagerComponent::SubtractItem(UInventoryCellWidget* ItemSubstract)
{
	FInventorySlotInfo ItemCash = ItemSubstract->GetItem();
	UInventoryComponent* InventoryComponent = ItemSubstract->GetParentInventory();
	const int32 LastIndex = ItemSubstract->IndexInInventory;
	ItemSubstract->Clear();
	
	ItemCash.Count -= 1;
	if(ItemCash.Count>0)
	{
		InventoryComponent->SetItem(LastIndex,ItemCash);
		ItemSubstract->AddItem(ItemCash,*GetItemData(ItemCash.Id),LastIndex);
	}else
	{
		InventoryComponent->ClearItem(LastIndex);
		ItemSubstract->Clear();
	}
}

void UInventoryManagerComponent::OnItemDropped(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	
	if(From == nullptr||To==nullptr)
	{
		
		return;
	}

	auto* FromInventory = From->GetParentInventory();
	auto* ToInventory = To->GetParentInventory();

	if(FromInventory == nullptr||ToInventory==nullptr)
	{
		return;
	}
	
	FInventorySlotInfo ItemCashToSlot = To->GetItem();
	FInventorySlotInfo ItemCashFromSlot = From->GetItem();
	if(ItemCashFromSlot.Count<=0)
	{
		return;
	}

	const int32 indexInInventoryNewItemTo = To->IndexInInventory;
	const int32 indexInInventoryNewItemFrom = From->IndexInInventory;
	//EQUIP
	if(UTankEquipInventoryComponent* EquipInventoryComponent = Cast<UTankEquipInventoryComponent>(To->GetParentInventory()))
	{
		
		if(To->EquipSlot == GetItemData(From->GetItem().Id)->EquipSlot)
		{
			if(To->IsClear())//To=ClearCell
			{
				FInventorySlotInfo newItemFromCount = ItemCashFromSlot;
				newItemFromCount.Count = 1;
			
				AddItem(To, newItemFromCount,indexInInventoryNewItemTo);
				SubtractItem(From);
				
				return;
			}
			else if(From->GetItem().Id == To->GetItem().Id) // ToItem==FromItem
			{
				return;
			}
			else if(From->GetItem().Id != To->GetItem().Id) // Change ToItem
			{
				
				if(ItemCashFromSlot.Count>1)
				{
					UInventoryCellWidget* Widget =	FindClearSlot();
					if(!Widget)
					{
						return;
					}
					Widget->GetParentInventory()->SetItem(Widget->IndexInInventory, To->GetItem());
					Widget->AddItem(To->GetItem(), *GetItemData(To->GetItem().Id),Widget->IndexInInventory);
				}
				else 
				{
					From->Clear();
					FromInventory->ClearItem(indexInInventoryNewItemFrom);
					AddItem(From,ItemCashToSlot ,indexInInventoryNewItemFrom);
				}
				To->Clear();
				ToInventory->ClearItem(indexInInventoryNewItemTo);
				AddItem(To,ItemCashFromSlot,indexInInventoryNewItemTo);
			}
		}
		return;
	}
	//ToPlayerInventory
	if(From->GetItem().Id == To->GetItem().Id) // stack
	{
		const int32 MaxAmountItem = GetItemData(ItemCashToSlot.Id)->MaxAmount;
		const int32 NowAmountItem = ItemCashFromSlot.Count+ItemCashToSlot.Count;
		
		const int32 remainderAmount = NowAmountItem-MaxAmountItem;
		
		ItemCashFromSlot.Count = remainderAmount;
		
		From->Clear();
		
		if(remainderAmount>0)
		{
			FromInventory->ClearItem(indexInInventoryNewItemFrom);
			FromInventory->SetItem(indexInInventoryNewItemFrom, ItemCashFromSlot);
			From->AddItem(ItemCashFromSlot, *GetItemData(ItemCashFromSlot.Id),indexInInventoryNewItemFrom);
			
			ItemCashToSlot.Count = MaxAmountItem;
		}
		else
		{
			ItemCashToSlot.Count = NowAmountItem;
		}
		FromInventory->ClearItem(indexInInventoryNewItemFrom);
		ToInventory->ClearItem(indexInInventoryNewItemTo);
		AddItem(To, ItemCashToSlot,indexInInventoryNewItemTo);
	}
	else if(To->IsClear()) //change from to clear
	{
		ToInventory->ClearItem(indexInInventoryNewItemTo);
		AddItem(To, ItemCashFromSlot,indexInInventoryNewItemFrom);
		
		From->Clear();
		FromInventory->ClearItem(indexInInventoryNewItemFrom);
	}
	else //Change from in to and to in from
	{
		if(UTankEquipInventoryComponent* EquipInventoryComponent = Cast<UTankEquipInventoryComponent>(FromInventory))//проверка переносим ли предмет из эквипа
		{
			if(From->EquipSlot == GetItemData(To->GetItem().Id)->EquipSlot) // можно ли поместить предмет на который бросили в слот эквипа
			{
				ToInventory->ClearItem(indexInInventoryNewItemTo);
				AddItem(To, ItemCashFromSlot,indexInInventoryNewItemTo);
				FromInventory->ClearItem(indexInInventoryNewItemFrom);
				AddItem(From, ItemCashToSlot,indexInInventoryNewItemFrom);
			}
			else
			{
				UInventoryCellWidget* Widget =	FindClearSlot();
				if(!Widget)
				{
					return;
				}
				Widget->GetParentInventory()->SetItem(Widget->IndexInInventory, From->GetItem());
				Widget->AddItem(From->GetItem(), *GetItemData(From->GetItem().Id),Widget->IndexInInventory);
				From->Clear();
				FromInventory->ClearItem(indexInInventoryNewItemFrom);
				return;
			}
		}
		ToInventory->ClearItem(indexInInventoryNewItemTo);
		AddItem(To, ItemCashFromSlot,indexInInventoryNewItemTo);
		FromInventory->ClearItem(indexInInventoryNewItemFrom);
		AddItem(From, ItemCashToSlot,indexInInventoryNewItemFrom);
	}
}

void UInventoryManagerComponent::AddItem(UInventoryCellWidget* Widget, FInventorySlotInfo& InfoSlot, int32 Index)
{
	Widget->Clear();
	Widget->AddItem(InfoSlot,*GetItemData(InfoSlot.Id),Index);
	Widget->GetParentInventory()->SetItem(Index, InfoSlot);
}