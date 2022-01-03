// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FInventoryItemInfo.h"
#include "UObject/Interface.h"
#include "EquipInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UEquipInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYPLUGIN_API IEquipInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Test Interface")
	void EquipItem(EEquipSlot Slot, FName ItemID);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Test Interface")
	void UnequipItem(EEquipSlot Slot, FName ItemID);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Test Interface")
	void UseItem(FName ItemID);
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
