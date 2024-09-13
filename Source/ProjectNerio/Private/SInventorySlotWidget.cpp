﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SInventorySlotWidget.h"

#include "SDragItemVisual.h"
#include "SInventoryComponent.h"
#include "SItemDrag.h"
#include "SQuantitySelectionWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComp = GetOwningPlayerPawn()->GetComponentByClass<USInventoryComponent>();
}

void USInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG(LogTemp, Log, TEXT("This is the InventorySlotWidget NativeConstruct"));
}

void USInventorySlotWidget::UpdateSlot() const
{
	if (InventoryComp->IsIndexEmpty(UISlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Index is empty: %d"), UISlotIndex)
		SlotIcon->SetVisibility(ESlateVisibility::Collapsed);
		AmountText->SetText(FText::GetEmpty());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Index is not empty, set the button and icon: %d"), UISlotIndex)
		auto& [Item, Quantity, SlotIndex] = InventoryComp->GetItemAtIndex(UISlotIndex);
		UE_LOG(LogTemp, Warning, TEXT("Index is not empty, set the button and icon: %d"), SlotIndex)
		AmountText->SetText(FText::AsNumber(Quantity));
		SlotIcon->SetBrushFromTexture(Item.Icon);
		SlotIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		GetTextAmountVisibility(SlotIndex);
	}
}

void USInventorySlotWidget::GetTextAmountVisibility(const int32 Index) const
{
	if (!InventoryComp->InventoryArray[Index].Item.bIsStackable)
	{
		AmountText->SetText(FText::GetEmpty());
	}
	else
	{
		AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void USInventorySlotWidget::SetSlotIndex(const int32 Index)
{
	UISlotIndex = Index;
}

void USInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	USItemDrag* DragDropOp = NewObject<USItemDrag>();
	if (DragItemVisualClass && SlotIcon)
	{
		const TObjectPtr<USDragItemVisual> DragItemVisual = CreateWidget<USDragItemVisual>(this, DragItemVisualClass);
		UTexture2D* IconTexture = Cast<UTexture2D>(SlotIcon->GetBrush().GetResourceObject());

		DragItemVisual->ItemIcon->SetBrushFromTexture(IconTexture);
		DragItemVisual->ItemBorder->SetBrushColor(FLinearColor::Gray);

		DragDropOp->DraggedWidgetReference = this;
		DragDropOp->ItemImage = IconTexture;
		DragDropOp->SlotIndex = UISlotIndex;
		DragDropOp->DefaultDragVisual = DragItemVisual;

		DragDropOp->OnDragCancelled.AddDynamic(this, &USInventorySlotWidget::OnDragCanceled);
		OutOperation = DragDropOp;
	}
}

void USInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

FReply USInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	
	return Reply.Unhandled();
}

bool USInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (const TObjectPtr<USItemDrag> ItemDrag = Cast<USItemDrag>(InOperation))
	{
		int32 DraggedIndex = ItemDrag->SlotIndex;
		int32 DroppedIndex = UISlotIndex;

		UE_LOG(LogTemp, Log, TEXT("Before Swap - Source Index: %d, Destination Index: %d"), DraggedIndex, DroppedIndex);

		// Swap items between slots
		if (InventoryComp->InventoryArray[DraggedIndex].Item.ItemClass == InventoryComp->InventoryArray[DroppedIndex].Item.ItemClass)
		{
			InventoryComp->StackItems(DraggedIndex, DroppedIndex);
		}
		else
		{
			InventoryComp->SwapItems(DraggedIndex, DroppedIndex);
		}
		

		UE_LOG(LogTemp, Log, TEXT("After Swap - Source Index: %d, Destination Index: %d"), DraggedIndex, DroppedIndex);

		return true;
	}
	

	return false;
}

void USInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bIsInventoryWidget = true;

	UE_LOG(LogTemp, Log, TEXT("IsInventoryWidget: %s"), 
			bIsInventoryWidget ? TEXT("True") : TEXT("False"));
}

void USInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsInventoryWidget = false;
	
	UE_LOG(LogTemp, Log, TEXT("IsInventoryWidget: %s"), 
			bIsInventoryWidget ? TEXT("True") : TEXT("False"));
}

void USInventorySlotWidget::OnDragCanceled(UDragDropOperation* DropOperation)
{
	DropItemsOutsideInventory();
}

void USInventorySlotWidget::DropItemsOutsideInventory()
{
	if (InventoryComp && !InventoryComp->IsIndexEmpty(UISlotIndex))
	{
		 // Create and show the quantity selection widget
		 if (USQuantitySelectionWidget* QuantityWidget = CreateWidget<USQuantitySelectionWidget>(GetWorld(), QuantitySelectionWidgetClass))
		 {
		 	QuantityWidget->SetInventorySlot(this);
		 	QuantityWidget->AddToViewport();
		 }
	}
}
