// Fill out your copyright notice in the Description page of Project Settings.


#include "SInventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "SInventoryComponent.h"
#include "SInventorySlotWidget.h"

void USInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComp = GetOwningPlayerPawn()->GetComponentByClass<USInventoryComponent>();
	SlotsPerRow = 5;

	UE_LOG(LogTemp, Log, TEXT("Before GenerateSlotWidgets"));
	GenerateSlotWidgets();
}

void USInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryComp)
	{
		InventoryComp->OnSlotUpdate.AddDynamic(this, &USInventoryWidget::OnSlotUpdated);
	}
}

void USInventoryWidget::GenerateSlotWidgets()
{
	SlotGrid->ClearChildren();
	SlotWidgets.Empty();

	for (int32 Index = 0; Index < InventoryComp->InventoryArray.Num(); ++Index)
	{
		if (USInventorySlotWidget* Widget = CreateWidget<USInventorySlotWidget>(GetWorld(), SlotWidgetClass))
		{
			Widget->SetSlotIndex(Index);
			SlotWidgets.Add(Widget);
			SlotGrid->AddChildToUniformGrid(Widget, FindRow(Index), FindColumn(Index));
			UE_LOG(LogTemp, Log, TEXT("Slot Widgets Are Added"));
		}
	}
}

void USInventoryWidget::OnSlotUpdated(int32 UpdatedIndex)
{
	if (SlotWidgets.IsValidIndex(UpdatedIndex))
	{
		if (USInventorySlotWidget* SlotWidget = Cast<USInventorySlotWidget>(SlotWidgets[UpdatedIndex]))
		{
			SlotWidget->UpdateSlot();
		}
	}
}

int32 USInventoryWidget::FindColumn(const int32 ArrayIndex) const
{
	const int32 Remainder = FMath::Modulo(ArrayIndex, SlotsPerRow);
	const int32 Result = trunc(Remainder);

	return Result;
}

int32 USInventoryWidget::FindRow(const int32 ArrayIndex) const
{
	const int32 Division = ArrayIndex / SlotsPerRow;
	const int32 Result = trunc(Division);

	return Result;
}
