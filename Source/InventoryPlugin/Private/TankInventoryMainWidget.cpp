// Fill out your copyright notice in the Description page of Project Settings.


#include "TankInventoryMainWidget.h"
#include "InventoryManagerComponent.h"


void UTankInventoryMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UTankInventoryMainWidget::CloseInventory()
{
	ManagerComponent->OpenInventory(true);
}
inline FReply UTankInventoryMainWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey()==EKeys::I)
	{
		CloseInventory();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}