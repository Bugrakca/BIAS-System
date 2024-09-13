// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInventorySlotWidget.generated.h"

class USQuantitySelectionWidget;
class USDragItemVisual;
class UTextBlock;
class UImage;
class UButton;
class USInventoryComponent;
class USInventoryWidget;
/**
 * 
 */
UCLASS()
class PROJECTNERIO_API USInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> SlotIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmountText;
	
	UPROPERTY()
	bool bIsInventoryWidget;

	UPROPERTY()
	bool bIsStorageWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<USDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<USQuantitySelectionWidget> QuantitySelectionWidgetClass;

	// Functions
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	void GetTextAmountVisibility(int32 Index) const;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnDragCanceled(UDragDropOperation* DropOperation);

	void DropItemsOutsideInventory();

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USInventoryComponent> InventoryComp;
	
	UPROPERTY()
	int32 UISlotIndex;
	
	UFUNCTION()
	void UpdateSlot() const;

	UFUNCTION()
	void SetSlotIndex(int32 Index);

};
