// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCellWidget.h"

#include "InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryComponent.h"
#include "InventoryDragDropOperation.h"

void UInventoryCellWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryCellWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UInventoryCellWidget::AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, int32 NewIndex)
{
	
	if(bHasItem)
	{
		return  false;
	}

	if(ItemImage)
	{
		ItemImage->SetBrushFromTexture(ItemInfo.Icon.Get());
		ItemImage->SetVisibility(ESlateVisibility::Visible);
	}

	if(CountText)
	{
		CountText->SetVisibility(ESlateVisibility::Visible);
		CountText->SetText(FText::FromString(FString::FromInt(Item.Count)));
	}
	
	bHasItem = true;
	StoredItem = Item;
	IndexInInventory = NewIndex;
	return true;
}
void UInventoryCellWidget::Clear()
{
	if(!bHasItem)
	{
		return;
	}
	if (ItemImage)
	{
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CountText)
	{
		CountText->SetVisibility(ESlateVisibility::Hidden);
	}

	bHasItem = false;
	StoredItem = FInventorySlotInfo();
}
const FInventorySlotInfo& UInventoryCellWidget::GetItem()
{
	return StoredItem;
}

UInventoryComponent* UInventoryCellWidget::GetParentInventory() const
{
	return  ParentInventoryWidget ? ParentInventoryWidget->ParentInventory : nullptr;
}

FReply UInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(bHasItem && bIsDraggable && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return FReply::Handled();
	
}

void UInventoryCellWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryDragDropOperation::StaticClass());
	if(OutOperation)
	{
		UInventoryDragDropOperation* InventoryDragDropOperation = Cast<UInventoryDragDropOperation>(OutOperation);
		
		InventoryDragDropOperation->SourceCell = this;
		InventoryDragDropOperation->DefaultDragVisual = this;
		
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
		
	}
	else
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

bool UInventoryCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* InventoryDragDropOperation = Cast<UInventoryDragDropOperation>(InOperation);
	if(InventoryDragDropOperation && InventoryDragDropOperation->SourceCell != this)
	{
		if(OnItemDrop.IsBound())
		{
			OnItemDrop.Broadcast(InventoryDragDropOperation->SourceCell,this);
		}
	}
	InventoryDragDropOperation->SourceCell->SetVisibility(ESlateVisibility::Visible);
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


void UInventoryCellWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	this->SetVisibility(ESlateVisibility::Visible);
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

FReply UInventoryCellWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && bHasItem)
	{
		OnItemUse.Broadcast(this);
	}
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}



