// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "TankEquipInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYPLUGIN_API UTankEquipInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	UTankEquipInventoryComponent();
	UPROPERTY(EditAnywhere);
	TMap<int32, EEquipSlot> EquipSlots;
	

	virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item) override;

	virtual void ClearItem(int32 SlotIndex) override;
};
