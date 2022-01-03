// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FInventoryItemInfo.h"
#include "InventoryCellWidget.generated.h"

class UInventoryCellWidget;
class UTextBlock;
class UImage;
class UInventoryWidget;
class UInventoryComponent;
struct FInventoryItemInfo;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, UInventoryCellWidget* , UInventoryCellWidget* );
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUse, UInventoryCellWidget*);

UCLASS()
class INVENTORYPLUGIN_API UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	bool HasItem(){return bHasItem;};
	bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, int32 NewIndex =-1);
	void Clear();

	const FInventorySlotInfo& GetItem();

	bool IsClear() const {return !bHasItem;}
		
	FOnItemDrop OnItemDrop;
	FOnItemUse OnItemUse;

	UPROPERTY(EditAnywhere)
	int32 IndexInInventory = -1;

	UPROPERTY(EditAnywhere)
	EEquipSlot EquipSlot = EEquipSlot::Es_None;

	UPROPERTY()
	UInventoryWidget* ParentInventoryWidget;

	UInventoryComponent* GetParentInventory() const;
protected:
	bool bHasItem;	

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ItemImage;
    
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CountText;

	UPROPERTY()
	FInventorySlotInfo StoredItem;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	
	UPROPERTY(EditDefaultsOnly)
	bool bIsDraggable = true;
	
};


