// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SQuantitySelectionWidget.generated.h"

class USInventoryComponent;
class USInventorySlotWidget;
class USpinBox;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTNERIO_API USQuantitySelectionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USpinBox> QuantitySpinBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CancelButton;

	UPROPERTY()
	USInventorySlotWidget* AssociatedSlot;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USInventoryComponent> InventoryComp;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnCancelClicked();

public:
	void SetInventorySlot(USInventorySlotWidget* SlotWidget);
};
