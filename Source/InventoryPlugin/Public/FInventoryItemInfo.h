// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FInventoryItemInfo.generated.h"

UENUM()
enum class EItemType : uint8
{
	It_Misc,
	It_Currency,
	It_Equip,
	It_Consumable
};

UENUM()
enum class EItemRarity : uint8
{
	Ir_Common,
	Ir_Rare,
	Ir_Epic,

};

UENUM()
enum class EEquipSlot : uint8
{
	Es_None,
	Es_Gun,
	Es_Head,
	Es_Armor,

};

USTRUCT(BlueprintType)
struct FInventoryItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "General")
	FName Id;

	UPROPERTY(EditDefaultsOnly, Category = "Consumables")
	int32 Heal  =0;
	
	UPROPERTY(EditDefaultsOnly, Category = "General")
	FText Name;

	UPROPERTY(EditDefaultsOnly, Category = "General")
	int32 MaxAmount =1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	TSoftObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 Damage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 Armor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 Intelligence;
	
	UPROPERTY(EditDefaultsOnly, Category = "Type")
	EItemType Type;

	UPROPERTY(EditDefaultsOnly, Category = "Type")
	EEquipSlot EquipSlot;
	
	UPROPERTY(EditDefaultsOnly, Category = "Type")
	EItemRarity Rare;

	
};

USTRUCT(BlueprintType)
struct FInventorySlotInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Id = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Count;

	
};