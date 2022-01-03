// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SlateDelegates.h"
#include "TankInventoryMainWidget.generated.h"

class UInventoryManagerComponent;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class INVENTORYPLUGIN_API UTankInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void CloseInventory();
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
public:

	UPROPERTY()
	UInventoryManagerComponent* ManagerComponent;
	
	FOnKeyDown InventoryOnKeyDown;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* EquipWidget;
};


