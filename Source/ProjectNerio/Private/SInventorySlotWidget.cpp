// Fill out your copyright notice in the Description page of Project Settings.


#include "SInventoryComponent.h"
#include "SInventorySlotWidget.h"
#include "SInventoryWidget.h"
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

	// InventoryComp->OnSlotUpdate.AddDynamic(this, &USInventorySlotWidget::UpdateSlot);

	UE_LOG(LogTemp, Log, TEXT("This is the InventorySlotWidget NativeConstruct"));
}

void USInventorySlotWidget::UpdateSlot()
{
	// TArray<FSlotData> SimilarItems = InventoryComp->FindSimilarItems(InventoryComp->InventoryArray, InventoryComp->GetItemAtIndex(Index).Item);

	if (InventoryComp->IsIndexEmpty(UISlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Index is empty: %d"), UISlotIndex)
		SlotButton->SetIsEnabled(false);
		SlotIcon->SetVisibility(ESlateVisibility::Collapsed);
		AmountText->SetText(FText::GetEmpty());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Index is not empty, set the button and icon: %d"), UISlotIndex)

		SlotButton->SetIsEnabled(true);
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
		AmountText->SetVisibility(ESlateVisibility::Visible);
	}
}

void USInventorySlotWidget::SetSlotIndex(const int32 Index)
{
	UISlotIndex = Index;
}
