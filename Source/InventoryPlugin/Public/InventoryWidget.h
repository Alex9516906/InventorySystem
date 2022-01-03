// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryCellWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "FInventoryItemInfo.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UInventoryComponent;
UCLASS()
class INVENTORYPLUGIN_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void Init(int32 ItemsNum);
	
	bool AddItem(const FInventorySlotInfo& InventorySlotInfo, const FInventoryItemInfo& ItemInfo, int32 SlotPosition = -1);

	FOnItemDrop OnItemDrop;
	FOnItemUse OnItemUse;
	
	UPROPERTY()
	UInventoryComponent* ParentInventory;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<UInventoryCellWidget*> CellWidgets;
	
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UUniformGridPanel* ItemCellsGrid;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemsInPow = 5;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;
	
	UPROPERTY(EditAnywhere,meta = (BindWidgetOptional))
	UInventoryCellWidget* GoldCell;
	
	UInventoryCellWidget * CreateCellWidget();
	
	void InitCell(UInventoryCellWidget * NewCell);
	void OnItemDropped(UInventoryCellWidget* From, UInventoryCellWidget* To);
	void OnItemUsed(UInventoryCellWidget* UsedItem);

	FNativeOnVisibilityChangedEvent OnVisibilityChangedEvent;
	
};




