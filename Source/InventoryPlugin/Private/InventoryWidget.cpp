// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryCellWidget.h"
#include "Components/UniformGridPanel.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for(auto* Cell: CellWidgets)
	{
		InitCell(Cell);
	}
	
}

void UInventoryWidget::Init(int32 ItemsNum)
{

	if(ItemCellsGrid)
	{
		ItemCellsGrid->ClearChildren();
		for(int32 i=0; i<ItemsNum; i++)
		{
			UInventoryCellWidget* Widget = CreateCellWidget();
			if(Widget)
			{
				Widget->IndexInInventory = i;
				ItemCellsGrid->AddChildToUniformGrid(Widget, i/ItemsInPow, i%ItemsInPow);
			}
		}
	}
}

bool UInventoryWidget::AddItem(const FInventorySlotInfo& InventorySlotInfo, const FInventoryItemInfo& ItemInfo,
	int32 SlotPosition)
{
	if (ItemInfo.Type == EItemType::It_Currency)
	{
		if (GoldCell)
		{
			return GoldCell->AddItem(InventorySlotInfo, ItemInfo, -1);
		}
		return false;
	}
    
	if (ItemCellsGrid)
	{
		UInventoryCellWidget * WidgetToAddItem = nullptr;
		UInventoryCellWidget ** WidgetToAddItemPtr = 
			CellWidgets.FindByPredicate([SlotPosition](UInventoryCellWidget * Widget)
			{
				return Widget && Widget->IndexInInventory == SlotPosition;
			});

		if (WidgetToAddItemPtr)
		{
			WidgetToAddItem = *WidgetToAddItemPtr;
		}
		else
		{
			for (UInventoryCellWidget * CellWidget : CellWidgets)
			{
				if (!CellWidget->HasItem())
				{
					WidgetToAddItem = CellWidget;
					break;
				}
			}
		}

		if (WidgetToAddItem)
		{
			return WidgetToAddItem->AddItem(InventorySlotInfo, ItemInfo,WidgetToAddItem->IndexInInventory);
		}
	}
	return false;

}

UInventoryCellWidget * UInventoryWidget::CreateCellWidget()
{
	if (CellWidgetClass)
	{
		UInventoryCellWidget * CellWidget = CreateWidget<UInventoryCellWidget>(this, 
			CellWidgetClass);
		CellWidgets.Add(CellWidget);
		InitCell(CellWidget);
		return CellWidget;
	}
	return nullptr;
}

void UInventoryWidget::InitCell(UInventoryCellWidget* NewCell)
{
	if(NewCell)
	{
		if(!NewCell->OnItemDrop.IsBound())
		{
			NewCell->OnItemDrop.AddUObject(this, &UInventoryWidget::OnItemDropped);
		}
		if(!NewCell->OnItemUse.IsBound())
		{
			NewCell->OnItemUse.AddUObject(this, &UInventoryWidget::OnItemUsed);
		}
		NewCell->ParentInventoryWidget = this;
	}
}

void UInventoryWidget::OnItemDropped(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	if(OnItemDrop.IsBound())
	{
		OnItemDrop.Broadcast(From,To);
	}
}
void UInventoryWidget::OnItemUsed(UInventoryCellWidget* UsedItem)
{
	if(OnItemUse.IsBound())
	{
		OnItemUse.Broadcast(UsedItem);
	}
}