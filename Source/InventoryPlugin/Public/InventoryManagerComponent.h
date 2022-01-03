
#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "TankEquipInventoryComponent.h"
#include "InventoryCellWidget.h"
#include "TankInventoryMainWidget.h"
#include "InventoryManagerComponent.generated.h"


class UInventoryWidget;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYPLUGIN_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	
	
public:
	UPROPERTY()
	UTankInventoryMainWidget* MainInventoryWidget;
    
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTankInventoryMainWidget> MainInventoryWidgetClass;
	
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
    
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
	UPROPERTY()
	UInventoryWidget* EquipInventoryWidget;
    
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> EquipInventoryWidgetClass;
	
	UPROPERTY(EditAnywhere)
	int32 MinInventorySize = 20;
	
	UInventoryManagerComponent();

	void Init(UInventoryComponent* InInventoryComponent);
	void EquipInit(UInventoryComponent* InInventoryComponent);

	const FInventoryItemInfo* GetItemData(const FName& InId) const;

	UFUNCTION(BlueprintCallable)
	void OpenInventory(const bool bOpen);
	
	UFUNCTION(BlueprintCallable)
	void SetTableToSpawn(UDataTable* Table);

	void SetLocalInventory(UInventoryComponent* InInventoryComponent);
	
	void SetLocalEquipInventory(UInventoryComponent* InInventoryComponent);
	
protected:
	UPROPERTY()
	UInventoryComponent* LocalInventory;
	
	UPROPERTY()
	UTankEquipInventoryComponent* EquipInventory;

	UPROPERTY(EditAnywhere)
	UDataTable* ItemsData;

	void SubtractItem(UInventoryCellWidget* ItemSubstract);
	
	void OnItemDropped(UInventoryCellWidget* From,UInventoryCellWidget* To);
	void AddItem(UInventoryCellWidget* Widget, FInventorySlotInfo& InfoSlot, int32 Index);

	void OnItemUsed(UInventoryCellWidget* UsedItem);

	UInventoryCellWidget* FindClearSlot();

};




