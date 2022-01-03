// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FInventoryItemInfo.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYPLUGIN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	bool bInit;
	
	UPROPERTY(EditAnywhere)
	TMap<int32, FInventorySlotInfo> Items;

public:
	void SpawnInventory();
	const FInventorySlotInfo* GetItem(int32 SlotIndex) const{return Items.Find(SlotIndex);}

	virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item);

	virtual void ClearItem(int32 SlotIndex);

	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& InItem);

	const TMap<int32, FInventorySlotInfo>& GetItems() const{return Items;}

	int32 GetItemNum() const {return Items.Num(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SpawnItemTable;
		
	bool IsInit() const{return bInit;}
	void SetInit(){bInit = true;}
	
};
