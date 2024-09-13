// Fill out your copyright notice in the Description page of Project Settings.


#include "SQuantitySelectionWidget.h"

#include "SInventoryComponent.h"
#include "SInventorySlotWidget.h"
#include "SItemDataStructs.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"

void USQuantitySelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &USQuantitySelectionWidget::OnConfirmClicked);
	}
	
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &USQuantitySelectionWidget::OnCancelClicked);
	}
}

void USQuantitySelectionWidget::SetInventorySlot(USInventorySlotWidget* SlotWidget)
{
	AssociatedSlot = SlotWidget;
	if (AssociatedSlot && QuantitySpinBox)
	{
		FSlotData& SlotData = AssociatedSlot->InventoryComp->GetItemAtIndex(AssociatedSlot->UISlotIndex);
		QuantitySpinBox->SetMinValue(1);
		QuantitySpinBox->SetMaxValue(SlotData.Quantity);
		QuantitySpinBox->SetValue(1);
	}
}

void USQuantitySelectionWidget::OnConfirmClicked()
{
	if (AssociatedSlot && QuantitySpinBox)
	{
		int32 QuantityToDrop = FMath::RoundToInt(QuantitySpinBox->GetValue());
		AssociatedSlot->InventoryComp->DropItem(AssociatedSlot->UISlotIndex, QuantityToDrop);
		AssociatedSlot->InventoryComp->OnSlotUpdate.Broadcast(AssociatedSlot->UISlotIndex);
	}
	
	RemoveFromParent();
}

void USQuantitySelectionWidget::OnCancelClicked()
{
	RemoveFromParent();
}
