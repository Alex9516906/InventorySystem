
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryCellWidget.h"
#include "InventoryDragDropOperation.generated.h"

UCLASS()
class INVENTORYPLUGIN_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UInventoryCellWidget* SourceCell;
	
	
};
